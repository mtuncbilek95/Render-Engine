#version 460

layout(location = 0) out vec4 outColor;

#define MAX_STEPS 500
#define MAX_DIST 500.
#define SURF_DIST .001

float sdBox( vec3 p, vec3 b )
{
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float GetDist(vec3 point)
{
    vec4 sphere = vec4(0, 1, 6, 1);
    float distanceSphere = length(point -sphere.xyz) - sphere.w;
    float distancePlane = point.y;

    float boxDist = sdBox(point - vec3(3, 1, 10), vec3(1));

    float calcDistance = min(boxDist, distancePlane);
    return calcDistance;
}

float RayMarch(vec3 rayOrigin, vec3 rayDistance)
{
    float distanceOrigin = 0.;

    for (int i =0; i < MAX_STEPS; i++) {
        vec3 pointP = rayOrigin + distanceOrigin * rayDistance;
        float distanceScene = GetDist(pointP);
        distanceOrigin += distanceScene;
        if (distanceScene < SURF_DIST || distanceOrigin > MAX_DIST) break;
    }

    return distanceOrigin;
}

vec3 GetNormal(vec3 p)
{
    float d = GetDist(p);
    vec2 e = vec2(.01, 0);

    vec3 n = d - vec3(GetDist(p-e.xyy), GetDist(p-e.yxy), GetDist(p-e.yyx));

    return normalize(n);
}

float GetLight(vec3 p)
{
    vec3 lightPos = vec3 (0, 5, 10);
    lightPos.xz += vec2(sin(32), cos(32)) *5.;
    vec3 lightVec = normalize(lightPos - p);
    vec3 normal = GetNormal(p);

    float dif = clamp(dot(normal, lightVec), 0., 1.);

    float d = RayMarch(p+normal * SURF_DIST *2., lightVec);
    if (d<length(lightPos - p)) dif *= .1;
    return dif;
}

void main() {
    vec2 uv = (gl_FragCoord.xy-0.5 * vec2(1600, 900))/900;
    uv.y *= -1.;
    vec3 col = vec3(0);

    vec3 RayOrigin = vec3(0, 3, 0);

    vec3 RayDirection = normalize(vec3(uv.x, uv.y, 1));

    float d = RayMarch(RayOrigin, RayDirection);

    vec3 p = RayOrigin + RayDirection * d;

    float diffuse = GetLight(p);

    col = vec3(diffuse);

    outColor = vec4(col, 1.0);
}
