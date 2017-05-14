#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

#ifdef MAINPROGRAM
vec3 eyeinit(0.0, 0.5, 5.0);
vec3 upinit(0.0, 1.0, 0.0);
int width = 640;
int height = 480;
float fovy = 90.0;
mat4 model = mat4(0);
#else
EXTERN vec3 eyeinit;
EXTERN vec3 upinit;
EXTERN vec3 center;
EXTERN int amountinit;
EXTERN int w, h;
EXTERN float fovy;
#endif 