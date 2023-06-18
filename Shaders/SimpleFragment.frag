#version 460

layout(location = 0) out vec4 outColor;

#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST .001

mat2 Rot(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}

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

    float boxDist = sdBox(point - vec3(0, 1, 0), vec3(1));

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
    vec3 lightPos = vec3 (4, 5, 3);
    //lightPos.xz += vec2(sin(iTime * 2.), cos(iTime * 2.)) *5.;
    vec3 lightVec = normalize(lightPos - p);
    vec3 normal = GetNormal(p);

    float dif = clamp(dot(normal, lightVec), 0., 1.);

    float d = RayMarch(p+normal * SURF_DIST *2., lightVec);
    if (d<length(lightPos - p)) dif *= .1;
    return dif;
}

vec3 R(vec2 uv, vec3 p, vec3 l, float z) {
    vec3 f = normalize(l-p),
    r = normalize(cross(vec3(0,1,0), f)),
    u = cross(f,r),
    c = p+f*z,
    i = c + uv.x*r + uv.y*u,
    d = normalize(i-p);
    return d;
}

void main() {
    vec2 uv = (gl_FragCoord.xy-0.5 * vec2(1600, 900))/900;
    uv.y *= -1.;
    vec3 col = vec3(0);

    vec3 RayOrigin = vec3(0, 4, -5);

/*    RayOrigin.yz *= Rot(-mouse.y);
    RayOrigin.xz *= Rot(iTime *.4 -mouse.x *6.2);*/

    vec3 RayDirection = R(uv, RayOrigin, vec3(0,0,0), .3);

    float d = RayMarch(RayOrigin, RayDirection);

    vec3 p = RayOrigin + RayDirection * d;

    float diffuse = GetLight(p);

    col = vec3(diffuse);

    outColor = vec4(col, 1.0);
}
