<?php
declare(strict_types=1);

$upstreamBase = 'http://104.234.41.94:15189/';
$allowedPaths = [
    'v1/Account/login',
    'v1/Account/register',
];

$requestUri = $_SERVER['REQUEST_URI'] ?? '';
$path = parse_url($requestUri, PHP_URL_PATH) ?: '';
$path = preg_replace('#^/api/#', '', $path);
$path = trim($path, '/');

if (!in_array($path, $allowedPaths, true)) {
    http_response_code(404);
    header('Content-Type: application/json');
    echo json_encode(['message' => 'Endpoint not found.']);
    exit;
}

$method = $_SERVER['REQUEST_METHOD'] ?? 'GET';

if ($method === 'OPTIONS') {
    http_response_code(204);
    exit;
}

if ($method !== 'POST') {
    http_response_code(405);
    header('Allow: POST, OPTIONS');
    header('Content-Type: application/json');
    echo json_encode(['message' => 'Method not allowed.']);
    exit;
}

$body = file_get_contents('php://input') ?: '';
$targetUrl = $upstreamBase . $path;

$headers = [
    'Content-Type: application/json',
    'Accept: application/json',
];

$ch = curl_init($targetUrl);
curl_setopt_array($ch, [
    CURLOPT_CUSTOMREQUEST => 'POST',
    CURLOPT_POSTFIELDS => $body,
    CURLOPT_HTTPHEADER => $headers,
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_HEADER => true,
    CURLOPT_CONNECTTIMEOUT => 10,
    CURLOPT_TIMEOUT => 30,
]);

$response = curl_exec($ch);

if ($response === false) {
    http_response_code(502);
    header('Content-Type: application/json');
    echo json_encode(['message' => 'API upstream unavailable.']);
    curl_close($ch);
    exit;
}

$headerSize = curl_getinfo($ch, CURLINFO_HEADER_SIZE);
$statusCode = curl_getinfo($ch, CURLINFO_RESPONSE_CODE) ?: 502;
$responseHeaders = substr($response, 0, $headerSize);
$responseBody = substr($response, $headerSize);
curl_close($ch);

http_response_code($statusCode);

foreach (explode("\r\n", $responseHeaders) as $headerLine) {
    if (stripos($headerLine, 'content-type:') === 0) {
        header($headerLine);
    }
}

if (!headers_sent()) {
    header('Content-Type: application/json');
}

echo $responseBody;
