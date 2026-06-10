#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <string>
#include <vector>

#include "../../LibProj/PathEngine/i_pathengine.h"

namespace
{
	struct Vec2
	{
		tSigned32 x;
		tSigned32 y;
	};

	struct Poly
	{
		std::vector<Vec2> v;
	};

	class FakeMesh;
	class FakeAgent;
	class FakePath;

	static iErrorHandler* g_errorHandler = 0;
	static const char* const g_versionAttributes[] =
	{
		"interface_major", "5",
		"interface_minor", "2",
		"bridge", "DMO x64 fake",
		0
	};

	template<class T>
	static void DestroySelf(T* self)
	{
		if(!self)
			return;
		self->~T();
		::operator delete(static_cast<void*>(self));
	}

	static cPosition MakePosition(tSigned32 x, tSigned32 y, tSigned32 cell = 0)
	{
		cPosition p;
		p.x = x;
		p.y = y;
		p.cell = cell;
		return p;
	}

	static cPosition InvalidPosition()
	{
		cPosition p;
		p.x = 0;
		p.y = 0;
		p.cell = -1;
		return p;
	}

	static double Distance(const cPosition& a, const cPosition& b)
	{
		const double dx = static_cast<double>(b.x) - static_cast<double>(a.x);
		const double dy = static_cast<double>(b.y) - static_cast<double>(a.y);
		return std::sqrt(dx * dx + dy * dy);
	}

	static tSigned32 RoundToPosition(double value)
	{
		const double maxValue = static_cast<double>((std::numeric_limits<tSigned32>::max)());
		const double minValue = static_cast<double>((std::numeric_limits<tSigned32>::min)());
		if(value > maxValue)
			return (std::numeric_limits<tSigned32>::max)();
		if(value < minValue)
			return (std::numeric_limits<tSigned32>::min)();
		return static_cast<tSigned32>(value >= 0.0 ? std::floor(value + 0.5) : std::ceil(value - 0.5));
	}

	static long long Cross(const Vec2& a, const Vec2& b, const Vec2& c)
	{
		const long long abx = static_cast<long long>(b.x) - a.x;
		const long long aby = static_cast<long long>(b.y) - a.y;
		const long long acx = static_cast<long long>(c.x) - a.x;
		const long long acy = static_cast<long long>(c.y) - a.y;
		return abx * acy - aby * acx;
	}

	static bool OnSegment(const Vec2& a, const Vec2& b, const Vec2& p)
	{
		if(Cross(a, b, p) != 0)
			return false;
		return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x)
			&& std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
	}

	static bool SegmentsIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
	{
		const long long c1 = Cross(a, b, c);
		const long long c2 = Cross(a, b, d);
		const long long c3 = Cross(c, d, a);
		const long long c4 = Cross(c, d, b);
		if(c1 == 0 && OnSegment(a, b, c))
			return true;
		if(c2 == 0 && OnSegment(a, b, d))
			return true;
		if(c3 == 0 && OnSegment(c, d, a))
			return true;
		if(c4 == 0 && OnSegment(c, d, b))
			return true;
		return ((c1 > 0) != (c2 > 0)) && ((c3 > 0) != (c4 > 0));
	}

	static bool PointInPoly(const Poly& poly, const Vec2& p)
	{
		if(poly.v.size() < 3)
			return false;

		bool inside = false;
		for(size_t i = 0, j = poly.v.size() - 1; i < poly.v.size(); j = i++)
		{
			const Vec2& a = poly.v[i];
			const Vec2& b = poly.v[j];
			if(OnSegment(a, b, p))
				return true;
			const bool crosses = ((a.y > p.y) != (b.y > p.y));
			if(crosses)
			{
				const double xAtY = static_cast<double>(b.x - a.x) * static_cast<double>(p.y - a.y)
					/ static_cast<double>(b.y - a.y) + static_cast<double>(a.x);
				if(static_cast<double>(p.x) < xAtY)
					inside = !inside;
			}
		}
		return inside;
	}

	static bool SegmentHitsPoly(const Poly& poly, const Vec2& a, const Vec2& b, tSigned32* lineOut)
	{
		if(PointInPoly(poly, a) || PointInPoly(poly, b))
		{
			if(lineOut && poly.v.size() >= 2)
			{
				lineOut[0] = poly.v[0].x;
				lineOut[1] = poly.v[0].y;
				lineOut[2] = poly.v[1].x;
				lineOut[3] = poly.v[1].y;
			}
			return true;
		}

		for(size_t i = 0; i < poly.v.size(); ++i)
		{
			const Vec2& c = poly.v[i];
			const Vec2& d = poly.v[(i + 1) % poly.v.size()];
			if(SegmentsIntersect(a, b, c, d))
			{
				if(lineOut)
				{
					lineOut[0] = c.x;
					lineOut[1] = c.y;
					lineOut[2] = d.x;
					lineOut[3] = d.y;
				}
				return true;
			}
		}
		return false;
	}

	static bool PolysOverlap(const Poly& a, const Poly& b, tSigned32* lineOut)
	{
		if(a.v.size() < 3 || b.v.size() < 3)
			return false;

		for(size_t i = 0; i < a.v.size(); ++i)
		{
			const Vec2& a0 = a.v[i];
			const Vec2& a1 = a.v[(i + 1) % a.v.size()];
			for(size_t j = 0; j < b.v.size(); ++j)
			{
				const Vec2& b0 = b.v[j];
				const Vec2& b1 = b.v[(j + 1) % b.v.size()];
				if(SegmentsIntersect(a0, a1, b0, b1))
				{
					if(lineOut)
					{
						lineOut[0] = b0.x;
						lineOut[1] = b0.y;
						lineOut[2] = b1.x;
						lineOut[3] = b1.y;
					}
					return true;
				}
			}
		}

		if(PointInPoly(b, a.v[0]))
		{
			if(lineOut && b.v.size() >= 2)
			{
				lineOut[0] = b.v[0].x;
				lineOut[1] = b.v[0].y;
				lineOut[2] = b.v[1].x;
				lineOut[3] = b.v[1].y;
			}
			return true;
		}

		if(PointInPoly(a, b.v[0]))
		{
			if(lineOut && b.v.size() >= 2)
			{
				lineOut[0] = b.v[0].x;
				lineOut[1] = b.v[0].y;
				lineOut[2] = b.v[1].x;
				lineOut[3] = b.v[1].y;
			}
			return true;
		}

		return false;
	}

	static bool ExtractQuoted(const std::string& xml, size_t tagStart, const char* attr, std::string& value)
	{
		const std::string needle = std::string(attr) + "=\"";
		size_t p = xml.find(needle, tagStart);
		if(p == std::string::npos)
			return false;
		size_t tagEnd = xml.find('>', tagStart);
		if(tagEnd != std::string::npos && p > tagEnd)
			return false;
		p += needle.size();
		const size_t q = xml.find('"', p);
		if(q == std::string::npos)
			return false;
		value.assign(xml.data() + p, q - p);
		return true;
	}

	static std::vector<tSigned32> ParseNumbers(const std::string& text)
	{
		std::vector<tSigned32> result;
		const char* s = text.c_str();
		while(*s)
		{
			while(*s && !std::isdigit(static_cast<unsigned char>(*s)) && *s != '-')
				++s;
			if(!*s)
				break;
			char* end = 0;
			const long v = std::strtol(s, &end, 10);
			result.push_back(static_cast<tSigned32>(v));
			s = end;
		}
		return result;
	}

	static bool ParsePositionXY(const std::string& text, tSigned32& x, tSigned32& y)
	{
		const size_t colon = text.find(':');
		const std::string xy = colon == std::string::npos ? text : text.substr(colon + 1);
		const std::vector<tSigned32> nums = ParseNumbers(xy);
		if(nums.size() < 2)
			return false;
		x = nums[0];
		y = nums[1];
		return true;
	}

	class FakeShape : public iShape
	{
	public:
		std::vector<Vec2> vertices;

		FakeShape(tSigned32 count, const tSigned32* data)
		{
			if(count > 0 && data)
			{
				vertices.reserve(static_cast<size_t>(count));
				for(tSigned32 i = 0; i < count; ++i)
					vertices.push_back(Vec2{ data[i * 2], data[i * 2 + 1] });
			}
		}

		void destroy()
		{
			DestroySelf(this);
		}

		tSigned32 size() const
		{
			return static_cast<tSigned32>(vertices.size());
		}

		void vertex(tSigned32 index, tSigned32& x, tSigned32& y) const
		{
			if(index < 0 || static_cast<size_t>(index) >= vertices.size())
			{
				x = 0;
				y = 0;
				return;
			}
			x = vertices[static_cast<size_t>(index)].x;
			y = vertices[static_cast<size_t>(index)].y;
		}
	};

	class FakeCollisionInfo : public iCollisionInfo
	{
	public:
		tSigned32 line[4];
		iAgent* agent;

		FakeCollisionInfo(const tSigned32* coords, iAgent* collidingAgent)
			: agent(collidingAgent)
		{
			for(int i = 0; i < 4; ++i)
				line[i] = coords ? coords[i] : 0;
		}

		void destroy()
		{
			DestroySelf(this);
		}

		void getCollidingLine(tSigned32* coords) const
		{
			if(!coords)
				return;
			for(int i = 0; i < 4; ++i)
				coords[i] = line[i];
		}

		iAgent* getCollidingAgent() const
		{
			return agent;
		}
	};

	class FakeObstacleSet : public iObstacleSet
	{
	public:
		std::vector<iAgent*> agents;

		void destroy()
		{
			DestroySelf(this);
		}

		void addAgent(iAgent* agent)
		{
			if(agent && !includes(agent))
				agents.push_back(agent);
		}

		void removeAgent(iAgent* agent)
		{
			agents.erase(std::remove(agents.begin(), agents.end(), agent), agents.end());
		}

		tSigned32 getNumberOfAgents() const
		{
			return static_cast<tSigned32>(agents.size());
		}

		iAgent* getAgent(tSigned32 index) const
		{
			if(index < 0 || static_cast<size_t>(index) >= agents.size())
				return 0;
			return agents[static_cast<size_t>(index)];
		}

		bool includes(iAgent* agent) const
		{
			return std::find(agents.begin(), agents.end(), agent) != agents.end();
		}

		tSigned32 size() const { return getNumberOfAgents(); }
		bool empty() const { return agents.empty(); }
		void updateCollisionPreprocessFor(const iShape*) const {}
		void updatePathfindingPreprocessFor(const iShape*) const {}
		tSigned32 getNumberOfConnectedRegions(const iShape*) const { return 1; }
		tSigned32 getConnectedRegionFor(const iShape*, const cPosition&) const { return 0; }
		tSigned32 getConnectedRegionForAgent(iAgent*) const { return 0; }
		void clear() { agents.clear(); }
		bool pathfindPreprocessNeedsUpdate(const iShape*) const { return false; }
		void savePathfindPreprocessFor(const iShape*, iOutputStream*) const {}
		void loadPathfindPreprocessFor(const iShape*, const char*, tUnsigned32) const {}
	};

	class FakeCollisionContext : public iCollisionContext
	{
	public:
		std::vector<iAgent*> agents;
		std::vector<iObstacleSet*> obstacleSets;
		cHorizontalRange queryBounds;
		bool hasBounds;

		FakeCollisionContext()
			: hasBounds(false)
		{
			queryBounds.minX = queryBounds.minY = queryBounds.maxX = queryBounds.maxY = 0;
		}

		void destroy()
		{
			DestroySelf(this);
		}

		void addAgent(iAgent* agent)
		{
			if(agent && !includes(agent))
				agents.push_back(agent);
		}

		void removeAgent(iAgent* agent)
		{
			agents.erase(std::remove(agents.begin(), agents.end(), agent), agents.end());
		}

		tSigned32 getNumberOfAgents() const
		{
			return static_cast<tSigned32>(agents.size());
		}

		iAgent* getAgent(tSigned32 index) const
		{
			if(index < 0 || static_cast<size_t>(index) >= agents.size())
				return 0;
			return agents[static_cast<size_t>(index)];
		}

		bool includes(iAgent* agent) const
		{
			return std::find(agents.begin(), agents.end(), agent) != agents.end();
		}

		void addObstacleSet(iObstacleSet* set)
		{
			if(set && std::find(obstacleSets.begin(), obstacleSets.end(), set) == obstacleSets.end())
				obstacleSets.push_back(set);
		}

		void removeObstacleSet(iObstacleSet* set)
		{
			obstacleSets.erase(std::remove(obstacleSets.begin(), obstacleSets.end(), set), obstacleSets.end());
		}

		tSigned32 getNumberOfObstacleSets() const
		{
			return static_cast<tSigned32>(obstacleSets.size());
		}

		iObstacleSet* getObstacleSet(tSigned32 index) const
		{
			if(index < 0 || static_cast<size_t>(index) >= obstacleSets.size())
				return 0;
			return obstacleSets[static_cast<size_t>(index)];
		}

		void updateCollisionPreprocessFor(const iShape*) const {}
		void updatePathfindingPreprocessFor(const iShape*) const {}
		void setSurfaceTypeTraverseCost(tSigned32, float) {}
		void setSurfaceTypeTraverseCostDirection(tSigned32, tSigned32, tSigned32) {}
		void setQueryBounds(const cHorizontalRange& bounds) { queryBounds = bounds; hasBounds = true; }
		void clearQueryBounds() { hasBounds = false; }
		void setOverlayConnectionPenalty(tSigned32, tSigned32) {}
		void clearOverlayConnectionPenalties() {}
		bool hasOverlayConnectionPenalties() const { return false; }
		tSigned32 getOverlayConnectionPenalty(tSigned32) const { return 0; }
		tSigned32 addRunTimeOffMeshConnection(const cPosition&, const cPosition&, tSigned32, tSigned32, tSigned32, tSigned32) { return 0; }
		void removeRunTimeOffMeshConnection(tSigned32) {}
		void clearRunTimeOffMeshConnections() {}
	};

	class FakePath : public iPath
	{
	public:
		FakeMesh* mesh;
		std::vector<cPosition> points;
		std::vector<tSigned32> connections;
		double preciseX;
		double preciseY;

		FakePath(FakeMesh* owner, const std::vector<cPosition>& pathPoints)
			: mesh(owner), points(pathPoints), preciseX(0.0), preciseY(0.0)
		{
			if(!points.empty())
			{
				preciseX = static_cast<double>(points[0].x);
				preciseY = static_cast<double>(points[0].y);
			}
			RebuildConnections();
		}

		void RebuildConnections()
		{
			const size_t count = points.size() > 0 ? points.size() - 1 : 0;
			connections.assign(count, 0);
		}

		void destroy()
		{
			DestroySelf(this);
		}

		tSigned32 size() const
		{
			return static_cast<tSigned32>(points.size());
		}

		cPosition position(tSigned32 pointIndex) const
		{
			if(pointIndex < 0 || static_cast<size_t>(pointIndex) >= points.size())
				return InvalidPosition();
			return points[static_cast<size_t>(pointIndex)];
		}

		tSigned32 connectionIndex(tSigned32 segmentIndex) const
		{
			if(segmentIndex < 0 || static_cast<size_t>(segmentIndex) >= connections.size())
				return -1;
			return connections[static_cast<size_t>(segmentIndex)];
		}

		const cPosition* getPositionArray() const
		{
			return points.empty() ? 0 : &points[0];
		}

		const tSigned32* getConnectionIndexArray() const
		{
			return connections.empty() ? 0 : &connections[0];
		}

		tUnsigned32 getLength() const
		{
			double length = 0.0;
			if(points.size() > 1)
			{
				const double firstDx = static_cast<double>(points[1].x) - preciseX;
				const double firstDy = static_cast<double>(points[1].y) - preciseY;
				length += std::sqrt(firstDx * firstDx + firstDy * firstDy);
				for(size_t i = 2; i < points.size(); ++i)
					length += Distance(points[i - 1], points[i]);
			}
			if(length < 0.0)
				return 0;
			if(length > static_cast<double>((std::numeric_limits<tUnsigned32>::max)()))
				return (std::numeric_limits<tUnsigned32>::max)();
			return static_cast<tUnsigned32>(length + 0.5);
		}

		iCollisionInfo* advanceAlong(const iShape*, float distance, const iCollisionContext*, float& precisionX, float& precisionY)
		{
			Advance(distance);
			precisionX = 0.0f;
			precisionY = 0.0f;
			return 0;
		}

		void renderOnGround(tSigned32, tSigned32, iRender3DLinesCallBack&) const {}
		iMesh* getMesh() const;

		void SyncHeadFromPrecise()
		{
			if(points.empty())
				return;
			points[0].x = RoundToPosition(preciseX);
			points[0].y = RoundToPosition(preciseY);
		}

		cPosition Advance(float distance)
		{
			if(points.empty())
				return InvalidPosition();

			double remaining = distance > 0.0f ? distance : 0.0;
			while(points.size() > 1 && remaining > 0.0)
			{
				const double dx = static_cast<double>(points[1].x) - preciseX;
				const double dy = static_cast<double>(points[1].y) - preciseY;
				const double segment = std::sqrt(dx * dx + dy * dy);
				if(segment <= 0.001 || remaining >= segment)
				{
					remaining -= segment;
					points.erase(points.begin());
					if(!points.empty())
					{
						preciseX = static_cast<double>(points[0].x);
						preciseY = static_cast<double>(points[0].y);
					}
					continue;
				}

				const double ratio = remaining / segment;
				preciseX += dx * ratio;
				preciseY += dy * ratio;
				SyncHeadFromPrecise();
				remaining = 0.0;
			}

			if(points.size() > 1 && getLength() == 0)
			{
				points.erase(points.begin(), points.end() - 1);
				if(!points.empty())
				{
					preciseX = static_cast<double>(points[0].x);
					preciseY = static_cast<double>(points[0].y);
				}
			}

			RebuildConnections();
			return points.empty() ? InvalidPosition() : points[0];
		}
	};

	static Poly ShapeAt(const iShape* shape, const cPosition& pos)
	{
		Poly poly;
		const FakeShape* fakeShape = static_cast<const FakeShape*>(shape);
		if(fakeShape)
		{
			poly.v.reserve(fakeShape->vertices.size());
			for(size_t i = 0; i < fakeShape->vertices.size(); ++i)
			{
				poly.v.push_back(Vec2{
					static_cast<tSigned32>(pos.x + fakeShape->vertices[i].x),
					static_cast<tSigned32>(pos.y + fakeShape->vertices[i].y)
				});
			}
		}
		return poly;
	}

	static tSigned32 ShapeClearance(const iShape* shape)
	{
		const FakeShape* fakeShape = static_cast<const FakeShape*>(shape);
		if(!fakeShape || fakeShape->vertices.empty())
			return 80;

		double maxDistance = 0.0;
		for(size_t i = 0; i < fakeShape->vertices.size(); ++i)
		{
			const double x = static_cast<double>(fakeShape->vertices[i].x);
			const double y = static_cast<double>(fakeShape->vertices[i].y);
			maxDistance = std::max(maxDistance, std::sqrt(x * x + y * y));
		}
		return static_cast<tSigned32>(std::max(80.0, maxDistance + 60.0));
	}

	class FakeAgent : public iAgent
	{
	public:
		FakeMesh* mesh;
		const iShape* shape;
		cPosition pos;
		void* userData;
		float traverseCost;
		tSigned32 traverseX;
		tSigned32 traverseY;
		bool movable;

		FakeAgent(FakeMesh* owner, const iShape* agentShape, const cPosition& position)
			: mesh(owner), shape(agentShape), pos(position), userData(0), traverseCost(1.0f),
			traverseX(0), traverseY(0), movable(true)
		{
		}

		void destroy()
		{
			DestroySelf(this);
		}

		iMesh* getMesh() const;
		const iShape* getShape() const { return shape; }
		void setUserData(void* voidPointer) { userData = voidPointer; }
		void* getUserData() const { return userData; }
		cPosition getPosition() const { return pos; }
		void moveTo(const cPosition& newPosition) { pos = newPosition; if(pos.cell == -1) pos.cell = 0; }
		bool canCollide() const { return true; }
		bool canPathfind() const { return true; }
		bool testCollisionAt(const iCollisionContext* context, const cPosition& newPosition) const;
		bool testCollisionTo(const iCollisionContext* context, const cPosition& newPosition) const;
		bool testCollisionTo_XY(const iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const;
		iCollisionInfo* firstCollisionTo(const iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const;
		cPosition findClosestUnobstructedPosition(const iCollisionContext* context, tSigned32 maximumDistance) const;
		void getAllAgentsOverlapped(const iCollisionContext*, iAgent** resultsBuffer, tSigned32& numberOverlapped) const
		{
			if(resultsBuffer && numberOverlapped > 0)
				resultsBuffer[0] = 0;
			numberOverlapped = 0;
		}
		bool testCollisionDirectlyAgainstPlacedShape(const iShape* otherShape, const cPosition& shapePosition) const
		{
			const Poly self = ShapeAt(shape, pos);
			const Poly other = ShapeAt(otherShape, shapePosition);
			return PolysOverlap(self, other, 0);
		}
		iPath* findShortestPathTo(const iCollisionContext* context, const cPosition& target) const;
		iPath* findShortestPathTo_WithQueryCallBack(const iCollisionContext* context, const cPosition& target, iQueryCallBack*) const
		{
			return findShortestPathTo(context, target);
		}
		iPath* findPathAway(const iCollisionContext*, const cPosition& awayFrom, tSigned32 distanceAway) const
		{
			cPosition target = pos;
			const double dx = static_cast<double>(pos.x) - awayFrom.x;
			const double dy = static_cast<double>(pos.y) - awayFrom.y;
			const double len = std::sqrt(dx * dx + dy * dy);
			if(len > 0.001)
			{
				target.x = static_cast<tSigned32>(static_cast<double>(pos.x) + dx / len * distanceAway);
				target.y = static_cast<tSigned32>(static_cast<double>(pos.y) + dy / len * distanceAway);
			}
			return findShortestPathTo(0, target);
		}
		iPath* findPathAway_WithQueryCallBack(const iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack*) const
		{
			return findPathAway(context, awayFrom, distanceAway);
		}
		iCollisionInfo* advanceAlongPath(iPath* path, float distance, const iCollisionContext*)
		{
			FakePath* fakePath = static_cast<FakePath*>(path);
			if(fakePath)
				pos = fakePath->Advance(distance);
			return 0;
		}
		iCollisionInfo* advanceAlongPathWithPrecision(iPath* path, float distance, const iCollisionContext* context, float& precisionX, float& precisionY)
		{
			precisionX = 0.0f;
			precisionY = 0.0f;
			return advanceAlongPath(path, distance, context);
		}
		iPath* generateCurvedPath(iPath* basePath, const iCollisionContext*, tSigned32, tSigned32, tSigned32, float, float) const { return basePath; }
		void setTraverseCost(float cost) { traverseCost = cost; }
		void setTraverseCostDirection(tSigned32 x, tSigned32 y) { traverseX = x; traverseY = y; }
		float getTraverseCost() const { return traverseCost; }
		void getTraverseCostDirection(tSigned32& x, tSigned32& y) const { x = traverseX; y = traverseY; }
		iPath* generateCurvedPath_WithEndVector(iPath* basePath, const iCollisionContext*, tSigned32, tSigned32, tSigned32, tSigned32, tSigned32, float, float) const { return basePath; }
		bool isMoveable() const { return movable; }
	};

	class FakeMesh : public iMesh
	{
	public:
		cHorizontalRange bounds;
		bool hasBounds;
		std::vector<Vec2> verts3D;
		std::vector<Poly> obstacles;

		FakeMesh(const char* data, tUnsigned32 size)
			: hasBounds(false)
		{
			bounds.minX = bounds.minY = bounds.maxX = bounds.maxY = 0;
			ParseXml(std::string(data ? data : "", data ? static_cast<size_t>(size) : 0));
		}

		void ParseXml(const std::string& xml)
		{
			size_t meshStart = xml.find("<mesh3D");
			size_t meshEnd = xml.find("</mesh3D>");
			if(meshStart == std::string::npos)
				meshStart = 0;
			if(meshEnd == std::string::npos)
				meshEnd = xml.size();

			for(size_t p = xml.find("<vert", meshStart); p != std::string::npos && p < meshEnd; p = xml.find("<vert", p + 5))
			{
				std::string sx;
				std::string sy;
				if(!ExtractQuoted(xml, p, "x", sx) || !ExtractQuoted(xml, p, "y", sy))
					continue;
				Vec2 v{ static_cast<tSigned32>(std::strtol(sx.c_str(), 0, 10)), static_cast<tSigned32>(std::strtol(sy.c_str(), 0, 10)) };
				verts3D.push_back(v);
				if(!hasBounds)
				{
					bounds.minX = bounds.maxX = v.x;
					bounds.minY = bounds.maxY = v.y;
					hasBounds = true;
				}
				else
				{
					bounds.minX = std::min(bounds.minX, v.x);
					bounds.maxX = std::max(bounds.maxX, v.x);
					bounds.minY = std::min(bounds.minY, v.y);
					bounds.maxY = std::max(bounds.maxY, v.y);
				}
			}

			for(size_t p = xml.find("<obstacle"); p != std::string::npos; p = xml.find("<obstacle", p + 9))
			{
				std::string posText;
				std::string verticesText;
				if(!ExtractQuoted(xml, p, "position", posText) || !ExtractQuoted(xml, p, "vertices", verticesText))
					continue;
				tSigned32 baseX = 0;
				tSigned32 baseY = 0;
				ParsePositionXY(posText, baseX, baseY);
				const std::vector<tSigned32> nums = ParseNumbers(verticesText);
				if(nums.size() < 6 || nums.size() % 2 != 0)
					continue;
				Poly poly;
				for(size_t i = 0; i + 1 < nums.size(); i += 2)
					poly.v.push_back(Vec2{ static_cast<tSigned32>(baseX + nums[i]), static_cast<tSigned32>(baseY + nums[i + 1]) });
				obstacles.push_back(poly);
			}

			if(!hasBounds)
			{
				bounds.minX = -1000000;
				bounds.minY = -1000000;
				bounds.maxX = 1000000;
				bounds.maxY = 1000000;
				hasBounds = true;
			}
		}

		bool InsideBounds(const cPosition& p) const
		{
			return p.cell != -1
				&& p.x >= bounds.minX && p.x <= bounds.maxX
				&& p.y >= bounds.minY && p.y <= bounds.maxY;
		}

		bool StaticBlocked(const cPosition& p) const
		{
			if(!InsideBounds(p))
				return true;
			const Vec2 point{ p.x, p.y };
			for(size_t i = 0; i < obstacles.size(); ++i)
			{
				if(PointInPoly(obstacles[i], point))
					return true;
			}
			return false;
		}

		bool StaticBlockedForShape(const iShape* shape, const cPosition& p, tSigned32* lineOut = 0) const
		{
			const Poly candidate = ShapeAt(shape, p);
			if(candidate.v.empty())
				return StaticBlocked(p);

			for(size_t i = 0; i < candidate.v.size(); ++i)
			{
				if(!InsideBounds(MakePosition(candidate.v[i].x, candidate.v[i].y)))
				{
					if(lineOut)
					{
						lineOut[0] = bounds.minX;
						lineOut[1] = bounds.minY;
						lineOut[2] = bounds.maxX;
						lineOut[3] = bounds.minY;
					}
					return true;
				}
			}

			for(size_t i = 0; i < obstacles.size(); ++i)
			{
				if(PolysOverlap(candidate, obstacles[i], lineOut))
					return true;
			}
			return false;
		}

		bool SegmentBlocked(const cPosition& start, const cPosition& end, tSigned32* lineOut) const
		{
			if(!InsideBounds(start) || !InsideBounds(end))
			{
				if(lineOut)
				{
					lineOut[0] = bounds.minX;
					lineOut[1] = bounds.minY;
					lineOut[2] = bounds.maxX;
					lineOut[3] = bounds.minY;
				}
				return true;
			}
			const Vec2 a{ start.x, start.y };
			const Vec2 b{ end.x, end.y };
			for(size_t i = 0; i < obstacles.size(); ++i)
			{
				if(SegmentHitsPoly(obstacles[i], a, b, lineOut))
					return true;
			}
			return false;
		}

		bool ContextPointBlocked(const iCollisionContext* context, const cPosition& p, const FakeAgent* ignore) const
		{
			const FakeCollisionContext* fakeContext = static_cast<const FakeCollisionContext*>(context);
			if(!fakeContext)
				return false;

			const Vec2 point{ p.x, p.y };
			for(size_t i = 0; i < fakeContext->agents.size(); ++i)
			{
				const FakeAgent* agent = static_cast<const FakeAgent*>(fakeContext->agents[i]);
				if(!agent || agent == ignore)
					continue;
				const Poly poly = ShapeAt(agent->shape, agent->pos);
				if(PointInPoly(poly, point))
					return true;
			}

			for(size_t setIndex = 0; setIndex < fakeContext->obstacleSets.size(); ++setIndex)
			{
				iObstacleSet* set = fakeContext->obstacleSets[setIndex];
				if(!set)
					continue;
				for(tSigned32 i = 0; i < set->getNumberOfAgents(); ++i)
				{
					const FakeAgent* agent = static_cast<const FakeAgent*>(set->getAgent(i));
					if(!agent || agent == ignore)
						continue;
					const Poly poly = ShapeAt(agent->shape, agent->pos);
					if(PointInPoly(poly, point))
						return true;
				}
			}
			return false;
		}

		bool ContextShapeBlocked(const iCollisionContext* context, const iShape* shape, const cPosition& p, const FakeAgent* ignore, tSigned32* lineOut = 0) const
		{
			const Poly candidate = ShapeAt(shape, p);
			if(candidate.v.empty())
				return ContextPointBlocked(context, p, ignore);

			const FakeCollisionContext* fakeContext = static_cast<const FakeCollisionContext*>(context);
			if(!fakeContext)
				return false;

			for(size_t i = 0; i < fakeContext->agents.size(); ++i)
			{
				const FakeAgent* agent = static_cast<const FakeAgent*>(fakeContext->agents[i]);
				if(!agent || agent == ignore)
					continue;
				const Poly other = ShapeAt(agent->shape, agent->pos);
				if(PolysOverlap(candidate, other, lineOut))
					return true;
			}

			for(size_t setIndex = 0; setIndex < fakeContext->obstacleSets.size(); ++setIndex)
			{
				iObstacleSet* set = fakeContext->obstacleSets[setIndex];
				if(!set)
					continue;
				for(tSigned32 i = 0; i < set->getNumberOfAgents(); ++i)
				{
					const FakeAgent* agent = static_cast<const FakeAgent*>(set->getAgent(i));
					if(!agent || agent == ignore)
						continue;
					const Poly other = ShapeAt(agent->shape, agent->pos);
					if(PolysOverlap(candidate, other, lineOut))
						return true;
				}
			}
			return false;
		}

		bool ContextSegmentBlocked(const iCollisionContext* context, const cPosition& start, const cPosition& end, const FakeAgent* ignore, tSigned32* lineOut = 0) const
		{
			const FakeCollisionContext* fakeContext = static_cast<const FakeCollisionContext*>(context);
			if(!fakeContext)
				return false;

			const Vec2 a{ start.x, start.y };
			const Vec2 b{ end.x, end.y };
			for(size_t i = 0; i < fakeContext->agents.size(); ++i)
			{
				const FakeAgent* agent = static_cast<const FakeAgent*>(fakeContext->agents[i]);
				if(!agent || agent == ignore)
					continue;
				const Poly other = ShapeAt(agent->shape, agent->pos);
				if(SegmentHitsPoly(other, a, b, lineOut))
					return true;
			}

			for(size_t setIndex = 0; setIndex < fakeContext->obstacleSets.size(); ++setIndex)
			{
				iObstacleSet* set = fakeContext->obstacleSets[setIndex];
				if(!set)
					continue;
				for(tSigned32 i = 0; i < set->getNumberOfAgents(); ++i)
				{
					const FakeAgent* agent = static_cast<const FakeAgent*>(set->getAgent(i));
					if(!agent || agent == ignore)
						continue;
					const Poly other = ShapeAt(agent->shape, agent->pos);
					if(SegmentHitsPoly(other, a, b, lineOut))
						return true;
				}
			}
			return false;
		}

		bool PointBlockedForShape(const iShape* shape, const iCollisionContext* context, const cPosition& p, const FakeAgent* ignore = 0) const
		{
			return StaticBlockedForShape(shape, p) || ContextShapeBlocked(context, shape, p, ignore);
		}

		bool SegmentBlockedForShape(const iShape* shape, const iCollisionContext* context, const cPosition& start, const cPosition& end, const FakeAgent* ignore = 0, tSigned32* lineOut = 0) const
		{
			if(PointBlockedForShape(shape, context, start, ignore) || PointBlockedForShape(shape, context, end, ignore))
				return true;

			if(SegmentBlocked(start, end, lineOut) || ContextSegmentBlocked(context, start, end, ignore, lineOut))
				return true;

			const Poly localShape = ShapeAt(shape, MakePosition(0, 0));
			for(size_t i = 0; i < localShape.v.size(); ++i)
			{
				const cPosition shiftedStart = MakePosition(start.x + localShape.v[i].x, start.y + localShape.v[i].y);
				const cPosition shiftedEnd = MakePosition(end.x + localShape.v[i].x, end.y + localShape.v[i].y);
				if(SegmentBlocked(shiftedStart, shiftedEnd, lineOut)
					|| ContextSegmentBlocked(context, shiftedStart, shiftedEnd, ignore, lineOut))
				{
					return true;
				}
			}

			return false;
		}

		iPath* BuildPath(const iShape* shape, const iCollisionContext* context, const cPosition& start, const cPosition& goal, const FakeAgent* ignore = 0) const
		{
			if(PointBlockedForShape(shape, context, start, ignore) || PointBlockedForShape(shape, context, goal, ignore))
				return 0;

			tSigned32 line[4] = { 0, 0, 0, 0 };
			if(!SegmentBlockedForShape(shape, context, start, goal, ignore, line))
			{
				std::vector<cPosition> pts;
				pts.push_back(start);
				pts.push_back(goal);
				return new FakePath(const_cast<FakeMesh*>(this), pts);
			}

			std::vector<cPosition> candidates;
			candidates.push_back(start);
			candidates.push_back(goal);
			const tSigned32 nudge = 80;
			for(size_t i = 0; i < obstacles.size(); ++i)
			{
				for(size_t j = 0; j < obstacles[i].v.size(); ++j)
				{
					const Vec2 v = obstacles[i].v[j];
					const cPosition p1 = MakePosition(v.x + nudge, v.y + nudge);
					const cPosition p2 = MakePosition(v.x + nudge, v.y - nudge);
					const cPosition p3 = MakePosition(v.x - nudge, v.y + nudge);
					const cPosition p4 = MakePosition(v.x - nudge, v.y - nudge);
					if(!PointBlockedForShape(shape, context, p1, ignore)) candidates.push_back(p1);
					if(!PointBlockedForShape(shape, context, p2, ignore)) candidates.push_back(p2);
					if(!PointBlockedForShape(shape, context, p3, ignore)) candidates.push_back(p3);
					if(!PointBlockedForShape(shape, context, p4, ignore)) candidates.push_back(p4);
				}
			}

			const size_t n = candidates.size();
			if(n < 2 || n > 256)
				return 0;

			const double inf = (std::numeric_limits<double>::max)() / 4.0;
			std::vector<double> dist(n, inf);
			std::vector<int> prev(n, -1);
			std::vector<unsigned char> used(n, 0);
			dist[0] = 0.0;

			for(size_t iter = 0; iter < n; ++iter)
			{
				size_t best = n;
				double bestDist = inf;
				for(size_t i = 0; i < n; ++i)
				{
					if(!used[i] && dist[i] < bestDist)
					{
						best = i;
						bestDist = dist[i];
					}
				}
				if(best == n || best == 1)
					break;
				used[best] = 1;
				for(size_t j = 0; j < n; ++j)
				{
					if(used[j] || best == j)
						continue;
					if(SegmentBlockedForShape(shape, context, candidates[best], candidates[j], ignore, 0))
						continue;
					const double edge = Distance(candidates[best], candidates[j]);
					if(dist[best] + edge < dist[j])
					{
						dist[j] = dist[best] + edge;
						prev[j] = static_cast<int>(best);
					}
				}
			}

			if(prev[1] == -1)
				return 0;

			std::vector<cPosition> reversed;
			for(int at = 1; at != -1; at = prev[static_cast<size_t>(at)])
				reversed.push_back(candidates[static_cast<size_t>(at)]);
			std::reverse(reversed.begin(), reversed.end());
			return new FakePath(const_cast<FakeMesh*>(this), reversed);
		}

		void destroy() { DestroySelf(this); }
		tSigned32 getNumberOf3DFaces() const { return verts3D.size() >= 3 ? 2 : 0; }
		tSigned32 get3DFaceAtPosition(const cPosition& position) const { return positionIsValid(position) ? 0 : -1; }
		void get3DFaceVertex(tSigned32, tSigned32 vertexIndexInFace, tSigned32& x, tSigned32& y, float& z) const
		{
			if(vertexIndexInFace < 0 || static_cast<size_t>(vertexIndexInFace) >= verts3D.size())
			{
				x = y = 0;
				z = 0.0f;
				return;
			}
			x = verts3D[static_cast<size_t>(vertexIndexInFace)].x;
			y = verts3D[static_cast<size_t>(vertexIndexInFace)].y;
			z = 0.0f;
		}
		tSigned32 get3DFaceConnection(tSigned32, tSigned32) const { return -1; }
		void get3DFaceNormal(tSigned32, float& x, float& y, float& z) const { x = 0.0f; y = 0.0f; z = 1.0f; }
		tSigned32 get3DFaceAttribute(tSigned32, tSigned32) const { return 0; }
		tSigned32 getNumberOfSections() const { return 1; }
		tSigned32 getSurfaceType(const cPosition&) const { return 0; }
		tSigned32 getSectionID(const cPosition&) const { return 0; }
		void storeAnchor(const char*, const cPosition&, tSigned32) {}
		cPosition retrieveAnchor(const char*, tSigned32& orientation) const { orientation = 0; return InvalidPosition(); }
		tSigned32 getNumberOfAnchors() const { return 0; }
		void retrieveAnchorByIndex(tSigned32, cPosition& position, tSigned32& orientation, const char*& id) const { position = InvalidPosition(); orientation = 0; id = ""; }
		void storeNamedObstacle(const char*, iAgent*) {}
		iAgent* retrieveNamedObstacle(const char*) const { return 0; }
		tSigned32 getNumberOfNamedObstacles() const { return 0; }
		void retrieveNamedObstacleByIndex(tSigned32, iAgent*& agent, const char*& id) const { agent = 0; id = ""; }
		cPosition positionFor3DPoint(const tSigned32* point) const
		{
			if(!point)
				return InvalidPosition();
			cPosition p = MakePosition(point[0], point[1]);
			return positionIsValid(p) ? p : InvalidPosition();
		}
		cPosition positionFor3DPointF(const float* point) const
		{
			if(!point)
				return InvalidPosition();
			cPosition p = MakePosition(static_cast<tSigned32>(point[0]), static_cast<tSigned32>(point[1]));
			return positionIsValid(p) ? p : InvalidPosition();
		}
		cPosition positionNear3DPoint(const tSigned32* point, tSigned32, tSigned32) const
		{
			if(!point)
				return InvalidPosition();
			cPosition p = MakePosition(point[0], point[1]);
			p.x = std::min(std::max(p.x, bounds.minX), bounds.maxX);
			p.y = std::min(std::max(p.y, bounds.minY), bounds.maxY);
			return positionIsValid(p) ? p : InvalidPosition();
		}
		cPosition positionInSection(tSigned32, tSigned32 x, tSigned32 y) const { cPosition p = MakePosition(x, y); return positionIsValid(p) ? p : InvalidPosition(); }
		cPosition positionInSectionFor3DPoint(tSigned32, const tSigned32* point) const { return positionFor3DPoint(point); }
		cPosition positionInSectionFor3DPointF(tSigned32, const float* point) const { return positionFor3DPointF(point); }
		cPosition positionInSectionNear3DPoint(tSigned32, const tSigned32* point, tSigned32 h, tSigned32 v) const { return positionNear3DPoint(point, h, v); }
		cPosition positionFor3DPoint_ExcludeTerrain(const tSigned32* point, tSigned32) const { return positionFor3DPoint(point); }
		cPosition generateRandomPosition() const { return MakePosition((bounds.minX + bounds.maxX) / 2, (bounds.minY + bounds.maxY) / 2); }
		cPosition generateRandomPositionLocally(const cPosition& centre, tSigned32) const { return positionIsValid(centre) ? centre : generateRandomPosition(); }
		cPosition generateRandomPositionInSection(tSigned32) const { return generateRandomPosition(); }
		tSigned32 heightAtPosition(const cPosition&) const { return 0; }
		float heightAtPositionF(const cPosition&) const { return 0.0f; }
		float heightAtPositionWithPrecision(const cPosition&, float, float) const { return 0.0f; }
		tSigned32 getCellForEndOfLine(const cPosition&, tSigned32 x, tSigned32 y) const
		{
			return positionIsValid(MakePosition(x, y)) ? 0 : -1;
		}
		bool positionIsValid(const cPosition& p) const { return InsideBounds(p) && !StaticBlocked(p); }
		void burnContextIntoMesh(const iCollisionContext*) {}
		void saveGround(const char*, bool, iOutputStream*) const {}
		void setTerrainCallBack(tSigned32, iTerrainCallBack*) {}
		tSigned32 addEndPoint(const cPosition&) { return 0; }
		tSigned32 addOffMeshConnection(tSigned32, tSigned32, tSigned32) { return 0; }
		tSigned32 getNumberOfEndPoints() const { return 0; }
		cPosition getEndPoint(tSigned32) const { return InvalidPosition(); }
		tSigned32 getNumberOfOffMeshConnections() const { return 0; }
		void getOffMeshConnectionInfo(tSigned32, tSigned32& from, tSigned32& to, tSigned32& penalty) const { from = to = penalty = 0; }
		void clearOffMeshConnectionsAndEndPoints() {}
		void generateCollisionPreprocessFor(const iShape*, const char *const*) {}
		void generatePathfindPreprocessFor(const iShape*, const char *const*) {}
		void releaseAllPreprocessFor(const iShape*) {}
		void releasePathfindPreprocessFor(const iShape*) {}
		void preprocessGenerationCompleted() {}
		bool shapeCanCollide(const iShape*) const { return true; }
		bool shapeCanPathfind(const iShape*) const { return true; }
		void saveCollisionPreprocessFor(const iShape*, iOutputStream*) const {}
		void savePathfindPreprocessFor(const iShape*, iOutputStream*) const {}
		void loadCollisionPreprocessFor(const iShape*, const char*, tUnsigned32) {}
		void loadPathfindPreprocessFor(const iShape*, const char*, tUnsigned32) {}
		iAgent* placeAgent(const iShape* shape, const cPosition& position) const
		{
			cPosition p = position;
			if(p.cell == -1)
				p.cell = 0;

			// SetShape() uses placeAgent() to register map-object collision
			// shapes. Those shapes can overlap the static mesh by design, so
			// reject only an invalid anchor position here and let collision
			// queries enforce the actual shape overlap rules.
			if(!InsideBounds(p))
				return 0;
			return new FakeAgent(const_cast<FakeMesh*>(this), shape, p);
		}
		iCollisionContext* newContext() const { return new FakeCollisionContext(); }
		iObstacleSet* newObstacleSet() const { return new FakeObstacleSet(); }
		bool testPointCollision(const iShape* shape, const iCollisionContext* context, const cPosition& position) const { return PointBlockedForShape(shape, context, position); }
		bool testLineCollision(const iShape* shape, const iCollisionContext* context, const cPosition& start, const cPosition& end) const { return SegmentBlockedForShape(shape, context, start, end, 0, 0); }
		bool testLineCollision_XY(const iShape* shape, const iCollisionContext* context, const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const
		{
			cell = positionIsValid(MakePosition(x, y)) ? 0 : -1;
			return testLineCollision(shape, context, start, MakePosition(x, y, cell));
		}
		iCollisionInfo* firstCollision(const iShape* shape, const iCollisionContext* context, const cPosition& start, tSigned32 x, tSigned32 y, tSigned32& cell) const
		{
			cell = positionIsValid(MakePosition(x, y)) ? 0 : -1;
			tSigned32 line[4] = { 0, 0, 0, 0 };
			if(!SegmentBlockedForShape(shape, context, start, MakePosition(x, y, cell), 0, line))
				return 0;
			return new FakeCollisionInfo(line, 0);
		}
		cPosition findClosestUnobstructedPosition(const iShape* shape, const iCollisionContext* context, const cPosition& position, tSigned32 maximumDistance) const
		{
			if(!PointBlockedForShape(shape, context, position))
				return position;
			for(tSigned32 r = 50; r <= maximumDistance; r += 50)
			{
				for(int i = 0; i < 16; ++i)
				{
					const double a = (3.14159265358979323846 * 2.0 * i) / 16.0;
					cPosition p = MakePosition(static_cast<tSigned32>(position.x + std::cos(a) * r), static_cast<tSigned32>(position.y + std::sin(a) * r));
					if(!PointBlockedForShape(shape, context, p))
						return p;
				}
			}
			return InvalidPosition();
		}
		void getAllAgentsOverlapped(const iShape*, const iCollisionContext*, const cPosition&, iAgent** resultsBuffer, tSigned32& numberOverlapped) const
		{
			if(resultsBuffer && numberOverlapped > 0)
				resultsBuffer[0] = 0;
			numberOverlapped = 0;
		}
		iPath* findShortestPath(const iShape* shape, const iCollisionContext* context, const cPosition& start, const cPosition& goal) const
		{
			return BuildPath(shape, context, start, goal);
		}
		iPath* findShortestPath_WithQueryCallBack(const iShape* shape, const iCollisionContext* context, const cPosition& start, const cPosition& goal, iQueryCallBack*) const
		{
			return findShortestPath(shape, context, start, goal);
		}
		iPath* findPathAway(const iShape* shape, const iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, tSigned32 distanceAway) const
		{
			cPosition target = start;
			const double dx = static_cast<double>(start.x) - awayFrom.x;
			const double dy = static_cast<double>(start.y) - awayFrom.y;
			const double len = std::sqrt(dx * dx + dy * dy);
			if(len > 0.001)
			{
				target.x = static_cast<tSigned32>(static_cast<double>(start.x) + dx / len * distanceAway);
				target.y = static_cast<tSigned32>(static_cast<double>(start.y) + dy / len * distanceAway);
			}
			return BuildPath(shape, context, start, target);
		}
		iPath* findPathAway_WithQueryCallBack(const iShape* shape, const iCollisionContext* context, const cPosition& start, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack*) const
		{
			return findPathAway(shape, context, start, awayFrom, distanceAway);
		}
		iPath* generateCurvedPath(const iShape*, iPath* basePath, const iCollisionContext*, tSigned32, tSigned32, tSigned32, float, float) const { return basePath; }
		iPath* constructPath(const cPosition* positionsBuffer, const tSigned32*, tSigned32 pathLength) const
		{
			if(!positionsBuffer || pathLength <= 0)
				return 0;
			std::vector<cPosition> pts(positionsBuffer, positionsBuffer + pathLength);
			return new FakePath(const_cast<FakeMesh*>(this), pts);
		}
		iPath* constructPath_Reversed(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathLength) const
		{
			iPath* path = constructPath(positionsBuffer, connectionIndicesBuffer, pathLength);
			FakePath* fakePath = static_cast<FakePath*>(path);
			if(fakePath)
			{
				std::reverse(fakePath->points.begin(), fakePath->points.end());
				fakePath->RebuildConnections();
			}
			return path;
		}
		void savePath(iPath*, iOutputStream*) const {}
		iPath* loadPath(const char*, tUnsigned32) const { return 0; }
		void renderLineOnGround(const cPosition&, tSigned32, tSigned32, tSigned32, tSigned32, iRender3DLinesCallBack&) const {}
		iObstacleSet* newObstacleSet_WithAttributes(const char *const*) const { return new FakeObstacleSet(); }
		cPosition positionInSectionInShape(tSigned32 sectionID, tSigned32 shapeOriginX, tSigned32 shapeOriginY, tSigned32, const tSigned32*) const
		{
			return positionInSection(sectionID, shapeOriginX, shapeOriginY);
		}
		iPath* generateCurvedPath_WithEndVector(const iShape*, iPath* basePath, const iCollisionContext*, tSigned32, tSigned32, tSigned32, tSigned32, tSigned32, float, float) const { return basePath; }
		iAgent* placeLargeStaticObstacle(tSigned32 count, const tSigned32* data, const cPosition& position) const
		{
			FakeShape* shape = new FakeShape(count, data);
			return new FakeAgent(const_cast<FakeMesh*>(this), shape, position);
		}
		iAgent* placeProjected3DObstruction(const iFaceVertexMesh*, const cPosition&, tSigned32) const { return 0; }
		void addAnchorsAndShapes(const iAnchorsAndPinnedShapes*, const char*, tSigned32) {}
		void autoGenerateConnections(tSigned32, tSigned32, tSigned32, tSigned32, tSigned32, iConnectionTestCallBack&) {}
		tSigned32 getNumberOfBurntInObstacles() const { return static_cast<tSigned32>(obstacles.size()); }
		cPosition getBurntInObstacleRoot(tSigned32) const { return MakePosition(0, 0); }
		tSigned32 getBurntInObstacleVertices(tSigned32 index) const
		{
			if(index < 0 || static_cast<size_t>(index) >= obstacles.size())
				return 0;
			return static_cast<tSigned32>(obstacles[static_cast<size_t>(index)].v.size());
		}
		void getBurntInObstacleVertex(tSigned32 obstacleIndex, tSigned32 vertexIndex, tSigned32& x, tSigned32& y) const
		{
			if(obstacleIndex < 0 || static_cast<size_t>(obstacleIndex) >= obstacles.size()
				|| vertexIndex < 0 || static_cast<size_t>(vertexIndex) >= obstacles[static_cast<size_t>(obstacleIndex)].v.size())
			{
				x = y = 0;
				return;
			}
			const Vec2 v = obstacles[static_cast<size_t>(obstacleIndex)].v[static_cast<size_t>(vertexIndex)];
			x = v.x;
			y = v.y;
		}
		tSigned32 getNumberOfConnectedRegions(const iShape*) const { return 1; }
		tSigned32 getConnectedRegionFor(const iShape*, const cPosition&) const { return 0; }
		tSigned32 getConnectedRegionForAgent(iAgent*) const { return 0; }
		void clearAllNamedObstacles() {}
		bool positionIsBlockedBySurfaceTypeTraverseCosts(const iCollisionContext*, const cPosition& position) const { return StaticBlocked(position); }
		tSigned32 getFederationTileIndex() const { return 0; }
		tUnsigned32 calculatePathfindingCostForLine(const iShape* shape, const iCollisionContext* context, const cPosition& start, const cPosition& end, bool& blocked) const
		{
			blocked = SegmentBlockedForShape(shape, context, start, end, 0, 0);
			return blocked ? 0 : static_cast<tUnsigned32>(Distance(start, end) + 0.5);
		}
		void saveGroundEx(const char*, bool, bool, bool, iOutputStream*) const {}
	};

	iMesh* FakePath::getMesh() const
	{
		return reinterpret_cast<iMesh*>(mesh);
	}

	iMesh* FakeAgent::getMesh() const
	{
		return reinterpret_cast<iMesh*>(mesh);
	}

	bool FakeAgent::testCollisionAt(const iCollisionContext* context, const cPosition& newPosition) const
	{
		return mesh->PointBlockedForShape(shape, context, newPosition, this);
	}

	bool FakeAgent::testCollisionTo(const iCollisionContext* context, const cPosition& newPosition) const
	{
		return mesh->SegmentBlockedForShape(shape, context, pos, newPosition, this, 0);
	}

	bool FakeAgent::testCollisionTo_XY(const iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const
	{
		cell = mesh->positionIsValid(MakePosition(x, y)) ? 0 : -1;
		return testCollisionTo(context, MakePosition(x, y, cell));
	}

	iCollisionInfo* FakeAgent::firstCollisionTo(const iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const
	{
		cell = mesh->positionIsValid(MakePosition(x, y)) ? 0 : -1;
		tSigned32 line[4] = { 0, 0, 0, 0 };
		if(!mesh->SegmentBlockedForShape(shape, context, pos, MakePosition(x, y, cell), this, line))
			return 0;
		return new FakeCollisionInfo(line, 0);
	}

	cPosition FakeAgent::findClosestUnobstructedPosition(const iCollisionContext* context, tSigned32 maximumDistance) const
	{
		return mesh->findClosestUnobstructedPosition(shape, context, pos, maximumDistance);
	}

	iPath* FakeAgent::findShortestPathTo(const iCollisionContext* context, const cPosition& target) const
	{
		return mesh->BuildPath(shape, context, pos, target, this);
	}

	class FakePathEngine : public iPathEngine
	{
	public:
		const char *const* getVersionAttributes() const { return g_versionAttributes; }
		tSigned32 getInterfaceMajorVersion() const { return 5; }
		tSigned32 getInterfaceMinorVersion() const { return 2; }
		void getReleaseNumbers(tSigned32& major, tSigned32& minor, tSigned32& internal) const
		{
			major = 5;
			minor = 23;
			internal = 0;
		}
		void setErrorHandler(iErrorHandler* newHandler) { g_errorHandler = newHandler; }
		iErrorHandler* getErrorHandler() { return g_errorHandler; }
		void setPathPoolParameters(tSigned32, tSigned32) {}
		bool shapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData)
		{
			return numberOfPoints >= 3 && coordinateData != 0;
		}
		iShape* newShape(tSigned32 numberOfPoints, const tSigned32* coordinateData)
		{
			if(!shapeIsValid(numberOfPoints, coordinateData))
				return 0;
			return new FakeShape(numberOfPoints, coordinateData);
		}
		iMesh* loadMeshFromBuffer(const char* format, const char* dataBuffer, tUnsigned32 bufferSize, const char *const*)
		{
			if(!format || _stricmp(format, "xml") != 0 || !dataBuffer || bufferSize == 0)
				return 0;
			return new FakeMesh(dataBuffer, bufferSize);
		}
		iMesh* buildMeshFromContent(const iFaceVertexMesh* const*, tSigned32, const char *const*) { return 0; }
		void saveContentData(const iFaceVertexMesh* const*, tSigned32, const char*, iOutputStream*) {}
		tUnsigned32 totalMemoryAllocated() { return 0; }
		tUnsigned32 maximumMemoryAllocated() { return 0; }
		void resetMaximumMemoryAllocated() {}
		void setRandomSeed(tUnsigned32) {}
		bool collisionPreprocessVersionIsCompatible(const char*, tUnsigned32) { return true; }
		bool pathfindPreprocessVersionIsCompatible(const char*, tUnsigned32) { return true; }
		void deleteAllObjects() {}
		iContentChunk* newContentChunk(const iFaceVertexMesh*, const iAnchorsAndPinnedShapes*, const char *const*) { return 0; }
		iContentChunk* loadContentChunk(const char*, tUnsigned32) { return 0; }
		iMeshFederation* buildMeshFederation_FromWorldMesh(const iMesh*, tSigned32, tSigned32, iMeshFederationTileGeneratedCallBack&) { return 0; }
		iMeshFederation* loadFederation(const char*, const char*, tUnsigned32) { return 0; }
		iMeshFederation* buildMeshFederation_TilingOnly(const cHorizontalRange&, tSigned32, tSigned32) { return 0; }
		bool largeStaticObstacleShapeIsValid(tSigned32 numberOfPoints, const tSigned32* coordinateData, tSigned32, tSigned32) { return shapeIsValid(numberOfPoints, coordinateData); }
		iSourceTiling* buildRegularSourceTiling(const cHorizontalRange&, tSigned32) { return 0; }
		iMesh* buildMeshFromGroundTiles(const iSourceTiling*, const iMesh* const*, tSigned32, const char *const*) { return 0; }
		iSourceTiling* loadSourceTiling(const char*, const char*, tUnsigned32) { return 0; }
		void save2DContentSnapshot(const iFaceVertexMesh* const*, tSigned32, const char *const*, const char*, iOutputStream*) {}
		iSourceTiling* buildRegularSourceTiling_RoundUpForVoxels(const cHorizontalRange&, tSigned32, tSigned32) { return 0; }
		iMesh* buildMeshByCopyingConnectedGround(const iMesh*, const cPosition*, tUnsigned32, const char *const*) { return 0; }
	};

	static FakePathEngine g_engine;
}

extern "C" __declspec(dllexport) iPathEngine* __stdcall DLLExport_GetIPathEngine(iErrorHandler* handler)
{
	g_errorHandler = handler;
	return &g_engine;
}

extern "C" __declspec(dllexport) void* __stdcall DLLExport_RefWrapperPointer(void* pointer)
{
	return pointer;
}

extern "C" __declspec(dllexport) iPathEngine* __stdcall DllExport_GetIPathEngine(iErrorHandler* handler)
{
	return DLLExport_GetIPathEngine(handler);
}
