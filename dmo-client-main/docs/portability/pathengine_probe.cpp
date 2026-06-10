#include <windows.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

#include "../../LibProj/PathEngine/i_pathengine.h"

class ProbeErrorHandler : public iErrorHandler
{
public:
	eAction handle(const char* error_type, const char* error_string, const char* const* attributes)
	{
		(void)attributes;
		std::printf("error_type=%s error=%s\n", error_type ? error_type : "<null>", error_string ? error_string : "<null>");
		return CONTINUE;
	}
};

typedef iPathEngine* (__stdcall* GetPathEngineInterfaceFn)(iErrorHandler*);

static void RunQuery(iMesh* mesh, iShape* shape, iCollisionContext* context, const char* label, const cPosition& start, const cPosition& target)
{
	iAgent* agent = mesh->placeAgent(shape, start);
	std::printf("[%s] placeAgent agent=%p start=%d,%d,%d target=%d,%d,%d\n", label, agent, static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(start.cell), static_cast<int>(target.x), static_cast<int>(target.y), static_cast<int>(target.cell));
	if(agent == NULL)
		return;

	const bool targetBlocked = mesh->testPointCollision(shape, context, target);
	std::printf("[%s] testPointCollision targetBlocked=%d\n", label, targetBlocked ? 1 : 0);

	iPath* meshPath = mesh->findShortestPath(shape, context, start, target);
	std::printf("[%s] mesh.findShortestPath path=%p", label, meshPath);
	if(meshPath != NULL)
		std::printf(" size=%d length=%u", static_cast<int>(meshPath->size()), static_cast<unsigned>(meshPath->getLength()));
	std::printf("\n");

	iPath* path = agent->findShortestPathTo(context, target);
	std::printf("[%s] agent.findShortestPathTo path=%p", label, path);
	if(path != NULL)
		std::printf(" size=%d length=%u", static_cast<int>(path->size()), static_cast<unsigned>(path->getLength()));
	std::printf("\n");
}

static std::vector<char> ReadFileBytes(const char* path)
{
	std::ifstream input(path, std::ios::binary);
	if(!input)
		return std::vector<char>();
	input.seekg(0, std::ios::end);
	const std::streamoff size = input.tellg();
	input.seekg(0, std::ios::beg);
	std::vector<char> bytes(static_cast<size_t>(size));
	if(size > 0)
		input.read(&bytes[0], size);
	return bytes;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::printf("usage: pathengine_probe.exe <PathEngine.dll> [mesh.xml startX startY targetX targetY]\n");
		return 2;
	}

	HMODULE module = LoadLibraryA(argv[1]);
	if(module == NULL)
	{
		std::printf("LoadLibrary failed gle=%lu path=%s\n", GetLastError(), argv[1]);
		return 3;
	}

	const char* entryName = std::getenv("PE_ENTRY");
	FARPROC proc = entryName && entryName[0] ? GetProcAddress(module, entryName) : GetProcAddress(module, reinterpret_cast<LPCSTR>(1));
	if(proc == NULL)
	{
		std::printf("GetProcAddress %s failed gle=%lu\n", entryName && entryName[0] ? entryName : "ordinal 1", GetLastError());
		return 4;
	}

	ProbeErrorHandler handler;
	GetPathEngineInterfaceFn getInterface = reinterpret_cast<GetPathEngineInterfaceFn>(proc);
	iPathEngine* engine = getInterface(&handler);
	if(engine == NULL)
	{
		std::printf("getInterface returned null\n");
		return 5;
	}

	tSigned32 major = 0;
	tSigned32 minor = 0;
	tSigned32 internal = 0;
	engine->getReleaseNumbers(major, minor, internal);
	std::printf(
		"header=%d.%d runtime=%d.%d release=%d.%d.%d engine=%p\n",
		static_cast<int>(PATHENGINE_INTERFACE_MAJOR_VERSION),
		static_cast<int>(PATHENGINE_INTERFACE_MINOR_VERSION),
		static_cast<int>(engine->getInterfaceMajorVersion()),
		static_cast<int>(engine->getInterfaceMinorVersion()),
		static_cast<int>(major),
		static_cast<int>(minor),
		static_cast<int>(internal),
		engine);

	if(argc >= 7)
	{
		const char* meshPath = argv[2];
		const int startX = std::atoi(argv[3]);
		const int startY = std::atoi(argv[4]);
		const int targetX = std::atoi(argv[5]);
		const int targetY = std::atoi(argv[6]);
		std::vector<char> meshBytes = ReadFileBytes(meshPath);
		std::printf("mesh_file=%s bytes=%u query=%d,%d -> %d,%d\n", meshPath, static_cast<unsigned>(meshBytes.size()), startX, startY, targetX, targetY);
		if(meshBytes.empty())
			return 6;

		iMesh* mesh = engine->loadMeshFromBuffer("xml", &meshBytes[0], static_cast<tUnsigned32>(meshBytes.size()), 0);
		std::printf("loadMeshFromBuffer mesh=%p\n", mesh);
		if(mesh == NULL)
			return 7;

		const int mi = 20;
		const int mx = 60;
		tSigned32 shapeData[] =
		{
			 mi, mx,
			 mx, mi,
			 mx,-mi,
			 mi,-mx,
			-mi,-mx,
			-mx,-mi,
			-mx, mi,
			-mi, mx,
		};
		iShape* shape = engine->newShape(static_cast<tSigned32>(sizeof(shapeData) / sizeof(*shapeData) / 2), shapeData);
		std::printf("newShape shape=%p\n", shape);
		if(shape == NULL)
			return 8;

		iCollisionContext* context = mesh->newContext();
		std::printf("newContext context=%p\n", context);
		if(context == NULL)
			return 9;

		mesh->generateCollisionPreprocessFor(shape, 0);
		mesh->generatePathfindPreprocessFor(shape, 0);
		std::printf("preprocess_done\n");

		cPosition start;
		start.x = startX;
		start.y = startY;
		start.cell = 0;
		cPosition target;
		target.x = targetX;
		target.y = targetY;
		target.cell = 0;

		RunQuery(mesh, shape, context, "raw-cell0", start, target);

		tSigned32 startPoint[3] = { startX, startY, 0 };
		tSigned32 targetPoint[3] = { targetX, targetY, 0 };
		cPosition startFor3D = mesh->positionFor3DPoint(startPoint);
		cPosition targetFor3D = mesh->positionFor3DPoint(targetPoint);
		RunQuery(mesh, shape, context, "positionFor3DPoint", startFor3D, targetFor3D);

		cPosition startNear = mesh->positionNear3DPoint(startPoint, 2000, 2000);
		cPosition targetNear = mesh->positionNear3DPoint(targetPoint, 2000, 2000);
		RunQuery(mesh, shape, context, "positionNear3DPoint", startNear, targetNear);
	}
	return 0;
}
