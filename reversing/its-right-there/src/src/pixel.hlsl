// Manually remove symbols
#define pInput _0
#define pScreenSize _1
#define pFontTexture _2
#define FontTextureSampler _3
#define clightBase _4
#define clightDirectional _5
#define clightPoint _6
#define colour _7
#define direction _8

float4 pInput : register(c0);
float2 pScreenSize : register(c1);
Texture2D<float> pFontTexture : register(s0);

SamplerState FontTextureSampler
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};

#define CONFUSE 1.729304
#define _lR 0.*(CONFUSE)
#define _lt 1.*(CONFUSE)
#define _lE 2.*(CONFUSE)
#define _lF 3.*(CONFUSE)
#define _lA 4.*(CONFUSE)
#define _lT 5.*(CONFUSE)
#define _lq 6.*(CONFUSE)
#define _lp 7.*(CONFUSE)
#define _lk 8.*(CONFUSE)
#define _lV 9.*(CONFUSE)
#define _lj 10.*(CONFUSE)
#define _ly 11.*(CONFUSE)
#define _lM 12.*(CONFUSE)
#define _lB 13.*(CONFUSE)
#define _lK 14.*(CONFUSE)
#define _ln 15.*(CONFUSE)
#define _lw 16.*(CONFUSE)
#define _lh 17.*(CONFUSE)
#define _l2 18.*(CONFUSE)
#define _l5 19.*(CONFUSE)
#define _lL 20.*(CONFUSE)
#define _lr 21.*(CONFUSE)
#define _lY 22.*(CONFUSE)
#define _lx 23.*(CONFUSE)
#define _ll 24.*(CONFUSE)
#define _lb 25.*(CONFUSE)
#define _lg 26.*(CONFUSE)
#define _l0 27.*(CONFUSE)
#define _lv 28.*(CONFUSE)
#define _lU 29.*(CONFUSE)
#define _l3 30.*(CONFUSE)
#define _lcurlright 31.*(CONFUSE)
#define _li 32.*(CONFUSE)
#define _lunderscore 33.*(CONFUSE)
#define _lf 34.*(CONFUSE)
#define _lI 35.*(CONFUSE)
#define _lu 36.*(CONFUSE)
#define _le 37.*(CONFUSE)
#define _l8 38.*(CONFUSE)
#define _ld 39.*(CONFUSE)
#define _lo 40.*(CONFUSE)
#define _lX 41.*(CONFUSE)
#define _lC 42.*(CONFUSE)
#define _lcurlleft 43.*(CONFUSE)
#define _lH 44.*(CONFUSE)
#define _lW 45.*(CONFUSE)
#define _lN 46.*(CONFUSE)
#define _lS 47.*(CONFUSE)
#define _l9 48.*(CONFUSE)
#define _lP 49.*(CONFUSE)
#define _l4 50.*(CONFUSE)
#define _lG 51.*(CONFUSE)
#define _lz 52.*(CONFUSE)
#define _l6 53.*(CONFUSE)
#define _lJ 54.*(CONFUSE)
#define _ls 55.*(CONFUSE)
#define _l7 56.*(CONFUSE)
#define _lD 57.*(CONFUSE)
#define _lQ 58.*(CONFUSE)
#define _lc 59.*(CONFUSE)
#define _la 60.*(CONFUSE)
#define _lm 61.*(CONFUSE)
#define _l1 62.*(CONFUSE)
#define _lO 63.*(CONFUSE)

#define EP .001
#define SCENE_END 5.
#define TRACE_MAX_STEPS 200

#define PI 3.1415
#define sTime pInput.x*.1

#define SCENE_ID_DEFAULT 0
#define SCENE_ID_OBJECT 1
#define SCENE_ID_GROUND 2
#define SCENE_ID_LIGHT 3

struct Ray {
    float3 origin;
    float3 direction;
};

struct Camera {
    float3 pos;
    float3 lookat;
    float zoom;
    float fov;
};

struct LightBase {
    float3 colour;
};

struct LightDirectional {
    float3 direction;
    float3 colour;
};

struct LightPoint {
    float3 position;
    float3 colour;
};

LightBase clightBase = {1.*float3(0.302,1.000,0.859)};

LightDirectional clightDirectional = \
    {normalize(float3(1,1,1)), .3*float3(1., .2, .1)};
    
static LightPoint clightPoint = \
    {float3(0,0,0), .9*float3(1.,1.2,1.8)};

float2x2 rot(in float a) {
	float s = sin(a);
	float c = cos(a);
	return float2x2(c, -s, s, c);
}

float Primitive_sdBoxFrame(float3 p, float3 b, float e)
{
    // Inigo Quilez
    p = abs(p)-b;
    float3 q = abs(p+e)-e;
    return min(min(
    length(max(float3(p.x,q.y,q.z),0.0))+min(max(p.x,max(q.y,q.z)),0.0),
    length(max(float3(q.x,p.y,q.z),0.0))+min(max(q.x,max(p.y,q.z)),0.0)),
    length(max(float3(q.x,q.y,p.z),0.0))+min(max(q.x,max(q.y,p.z)),0.0));
}

float Primitive_sdSphere(in float3 p, in float radius)
{
    return length(p) - radius;
}

float Primitive_sdBox2D(in float2 p, in float2 b)
{
    float2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float Primitive_sdChar2D(float2 p, float c) 
{
    float w = .1;
    if (p.x<w|| p.x>1.-w || p.y<w|| p.y>1.-w) return Primitive_sdBox2D(p-float2(.5, .5), float2(.4-w/2., .4-w/2.));
    p.x = 1.-p.x;
    //p *= 64.;
    //float2 uv = (.5+floor(p))/512. + frac(floor(float2(c*16., 7.999-c)) / 8.);
    float2 uv = p / 8. + frac(floor(float2(c / (CONFUSE), 7.999 - c / 8. / (CONFUSE))) / 8.);
    uv.y = 1.-uv.y;
    return pFontTexture.SampleLevel(FontTextureSampler, uv, 0) - .5; // pFontTexture.Sample(FonstTextureSampler, uv);
}

float Primitive_sdChar3D(in float3 p, in float h, in float c)
{
    // Inigo Quilez
    p.xy += .5;
    float d = Primitive_sdChar2D(p.xy, c);
    float2 w = float2(d, abs(p.z) - h);
    return min(max(w.x,w.y),0.0) + length(max(w,0.0)) - .03;
}

float Primitive_smoothUnion(in float d1, in float d2, in float k)
{
    float h = clamp(0.5 + 0.5*(d2-d1)/k, 0.0, 1.0);
    return lerp(d2, d1, h) - k*h*(1.0-h); 
}

float Primitive_smoothIntersect(in float d1, in float d2, in float k)
{
    float h = clamp(0.5 - 0.5*(d2-d1)/k, 0.0, 1.0);
    return lerp(d2, d1, h) + k*h*(1.0-h); 
}
    
float Primitive_smoothSubtract(in float d1, in float d2, in float k)
{
    float h = clamp(0.5 - 0.5*(d2+d1)/k, 0.0, 1.0);
    return lerp(d2, -d1, h) + k*h*(1.0-h); 
}

float Object_sdBall(in float3 p)
{
    return Primitive_sdBoxFrame(p, float3(.6, .6, .6), .05) - .05;
}

float Object_sdFlag(in float3 p)
{
    float d = SCENE_END;
    const float flag[28] = {
        //_lS, 
        _lE - _lS,
        _lE - _lE,
        _lcurlleft - _lE,
        _lG - _lcurlleft,
        _lP - _lG,
        _lU - _lP,
        _lunderscore - _lU,
        _l0 - _lunderscore,
        _ln - _l0,
        _lL - _ln,
        _ly - _lL,
        _lunderscore - _ly,
        _lR - _lunderscore,
        _la - _lR,
        _ly - _la,
        _lt - _ly,
        _lr - _lt,
        _lA - _lr,
        _lC - _lA,
        _l3 - _lC,
        _lr - _l3,
        _lunderscore - _lr,
        _lp - _lunderscore,
        _l1 - _lp,
        _lx - _l1,
        _l3 - _lx,
        _ll - _l3,
        _lcurlright - _ll,
    };

    float c = _lS;
    int j = 0;
    for (int i = 0; i < 28; ++i) {
        float _d = Primitive_sdChar3D(p, .03, c);
        c += flag[j]; j += 1;
        d = min(_d, d);
        p.x += .5;
    }
    float _d = Primitive_sdChar3D(p, .03, c);
    d = min(_d, d);

    return d;
}

float Scene_sdf_Object(in float3 p)
{
    float3 _p = p;
    _p.x += .5;
    float flag = Object_sdFlag(_p);

    float sc; _p = p;
    _p.x -= .8;
    _p.yz = mul(_p.yz, rot(sTime)); _p.zx = mul(_p.zx, rot(sTime));
    float ball = Object_sdBall(_p);

    sc = .8;
    _p = p;
    _p.x -= .8;
    _p.yz = mul(_p.yz, rot(-sTime * .7 + .5)); _p.zx = mul(_p.zx, rot(sTime));
    ball = min(ball, sc * Object_sdBall(_p / sc));
    return min(flag, ball);
}

float Scene_sdf_Ground(in float3 p)
{
    // Optimization
    // float dp = p.y;
    // if (dp > 1.2) {return dp + 1.;}
    
    float ground = p.y + 1.1;
    float l = length(p.xz);
    // ground += sin(l*5. - sTime*4.) * .1*smoothstep(0.,2.,l);
    return ground;
}

float Scene_sdf_Light(in float3 p)
{
    p -= clightPoint.position;
    return Primitive_sdSphere(p, .1);
}

float Scene_sdf(in float3 p)
{
    float object = Scene_sdf_Object(p);
    float ground = Scene_sdf_Ground(p);
    float light = Scene_sdf_Light(p);
    
    float sdf = min(object, ground);
    sdf = min(sdf, light);
    return sdf;
}

float3 Scene_normal(in float3 p) {
	const float2 e = float2(EP, 0);
	return normalize(float3(Scene_sdf(p + e.xyy)-Scene_sdf(p - e.xyy), 
                          Scene_sdf(p + e.yxy)-Scene_sdf(p - e.yxy),	
                          Scene_sdf(p + e.yyx)-Scene_sdf(p - e.yyx)));
}

float _Scene_sdf(in float3 p, inout float d_light_min, inout float d_object_min)
{
    float object = Scene_sdf_Object(p);
    float ground = Scene_sdf_Ground(p);
    float light = Scene_sdf_Light(p);
    
    d_light_min = min(d_light_min, light);
    d_object_min = min(d_object_min, object);
    
    float sdf = min(object, ground);
    sdf = min(sdf, light);
    return sdf;
}

float Ray_trace(inout Ray ray, out float d_light_min, out float d_object_min, out int surface_id)
{
    float3 p = ray.origin;
    float3 d = ray.direction;
    float dist = 0.;
    d_light_min = SCENE_END;
    d_object_min = SCENE_END;
    
    for (int i = 0; i < TRACE_MAX_STEPS; ++i) {
        float3 _p = p + dist*d;
        float _d = _Scene_sdf(_p, d_light_min, d_object_min);
        dist += _d;
        if (_d < EP || dist > SCENE_END) break;
    }
    ray.origin = p + dist*d;
    
    //surface_id = SCENE_ID_DEFAULT;
    //if (Scene_sdf_Object(ray.origin) < EP) surface_id = SCENE_ID_OBJECT;
    //else if (Scene_sdf_Ground(ray.origin) < EP) surface_id = SCENE_ID_GROUND;
    //else if (Scene_sdf_Light(ray.origin) < EP) surface_id = SCENE_ID_LIGHT;
    surface_id = \
        (Scene_sdf_Object(ray.origin) < EP) * SCENE_ID_OBJECT + \
        (Scene_sdf_Ground(ray.origin) < EP) * SCENE_ID_GROUND + \
        (Scene_sdf_Light(ray.origin) < EP) * SCENE_ID_LIGHT;
    
    return dist;
}

float3 LightBase_computeLighting(in LightBase light)
{
    return light.colour;
}

float3 LightDirectional_computeLighting(in LightDirectional light, in float3 normal)
{
    float weight = dot(normal, light.direction);
    return light.colour * clamp(weight, 0., 1.);
}

float3 LightPoint_computeLighting(in LightPoint light, in float3 position, in float3 normal)
{
    float weight = dot(normal, normalize(light.position - position));
    return light.colour * clamp(weight, 0., 1.);
}

float3 LightPoint_computeShadow(in LightPoint light, in float3 position, in float3 normal)
{
    float3 v = light.position - position;
    float3 rd = normalize(v);
    float d_light_min, d_object_min;
    int surface_id;
    Ray ray = {position + rd*3.*EP/dot(rd, normal), rd};
    float dist = Ray_trace(ray, d_light_min, d_object_min, surface_id);
    
    // Compute soft shadows uwu
    float w = 3. * pow(length(v) / 20., 2.);
    float w2 = smoothstep(0., w, d_object_min);
    return float3(w2, w2, w2);
}

float3 LightPoint_computeBleed(in LightPoint light, in float3 position, in float3 direction, in float3 normal) 
{
    float x = -dot(normal, light.position - position);
        
    // Compute cheap bleed
    float w1 = .8;
    float w2 = .7;
    float bleed = w1 - clamp(x+(1.-w1), 0., 1.);
    float3 col = w2 * bleed * light.colour;

    // Compute cheap subscattering
    w1 = 1.;
    w2 = 4.;
    bleed = w1 - clamp(x+(1.-w1), .7, 1.);
    col += w2 * bleed*bleed*bleed * float3(1.,.1,.3);

    // Compute glare
    w1 = 1.;
    w2 = 5.;
    bleed = w1 - clamp(x+(1.-w1), 0., 1.);
    bleed *= 1.-smoothstep(0.,1.,
        length(
            dot(direction, light.position - position)*direction + position-light.position
        )
    );
    col += w2 * bleed*bleed*bleed * clightPoint.colour;
    
    return col;
}

float3 Ray_lighting(in Ray ray)
{
    float t = sTime*2.;
    clightPoint.position = float3(.8,0,0) + \
        .2*float3(sin(t*10.), sin(t*13.+5.), sin(t*17.+2.)) + \
        .02*float3(sin(t*50.), cos(t*57.+5.), sin(t*47.+2.));
    
    float d_light_min, d_object_min;
    int surface_id;
    float d = Ray_trace(ray, d_light_min, d_object_min, surface_id);
    
    float3 col = float3(0,0,0);
    
    if (surface_id == SCENE_ID_LIGHT) {
        col = clightPoint.colour;
        return col;
    }
    
    float3 normal = Scene_normal(ray.origin);
    
    col += LightPoint_computeLighting(clightPoint, ray.origin, normal);
    col *= LightPoint_computeShadow(clightPoint, ray.origin, normal);
        
    col += LightBase_computeLighting(clightBase);
    col += LightDirectional_computeLighting(clightDirectional, normal);
        
    col += LightPoint_computeBleed(clightPoint, ray.origin, ray.direction, normal);
    
    // Compute cheap distance fog
    float fog = 1. - smoothstep(1., 4., d);
    col *= fog*fog;
    col = clamp(col, 0., 1.);
    
    // Compute cheap light fog
    fog = clamp(.03/(d_light_min+.01), 0.,1.);
    col += fog * clightPoint.colour;
    
    return col;
}

void Camera_init(inout Camera cam) 
{
    cam.pos = float3(0,0,-3.);
    cam.lookat = float3(0,0,0);
    cam.zoom = 1.;
    cam.fov = 1.;
}

void Camera_mouse(inout Camera cam, in float2 m)
{
    cam.pos.xz = mul(cam.pos.xz, rot(clamp(m.x, -PI/3, PI/3)));
    cam.pos.zy = mul(cam.pos.zy, rot(clamp(m.y, -PI/3, PI/3)));
    cam.pos.y = max(cam.pos.y, -1.);
}

Ray Camera_projectRay(in Camera cam, in float2 uv)
{
    float3 front = normalize(cam.lookat - cam.pos);
    float3 screen_origin = cam.pos + front * cam.zoom;
    float3 vert = float3(0,1.,0);
    float3 up = normalize(vert-front*dot(vert, front));
    float3 right = cross(front, up);
    
    uv *= cam.fov;
    float3 ro = screen_origin + uv.x * right + uv.y * up;
    float3 rd = normalize(ro - cam.pos);
    Ray ret = {ro, rd};
    return ret;
}

void Image_postProcessing(inout float3 col)
{
    // Brighten
    col += clightPoint.colour * .2;
    // Contrast!
    col = pow(clamp(col,0,1), 2.5);
    // Cinematic
    col += 1.*float3(.01,.05,.07);
}

float4 main(float2 u:TEXCOORD) :COLOR
{
    float2 m = pInput.yz;
    float ratio = pScreenSize.x / pScreenSize.y;
    u -= .5;
    m -= .5;
    u.x *= ratio;
    m.x *= ratio;
    u.y *= -1;
    m.x *= -1;

    Camera cam; Camera_init(cam);
    Camera_mouse(cam, m);
    Ray ray = Camera_projectRay(cam, u);
    
    float3 col = Ray_lighting(ray);
    Image_postProcessing(col);
    
    return float4(col, 1.);
};