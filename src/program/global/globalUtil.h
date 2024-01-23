namespace global {

#define GVAR_Impl(type, functionName) type& functionName() {            \
static type val;                                                        \
return val;                                                             \
}
#define GVARV_Impl(type, functionName, value) type& functionName() {    \
static type val = value;                                                \
return val;                                                             \
}
#define GBOOL_Impl(functionName) bool& functionName() {                 \
static bool val = false;                                                \
return val;                                                             \
}
#define GBOOLV_Impl(functionName, value) bool& functionName() {         \
static bool val = value;                                                \
return val;                                                             \
}
#define GINT_Impl(functionName) int& functionName() {                   \
static int val = 0;                                                     \
return val;                                                             \
}
#define GINTV_Impl(functionName, value) int& functionName() {           \
static int val = value;                                                 \
return val;                                                             \
}
#define GFLOAT_Impl(functionName) float& functionName() {               \
static float val = 0.f;                                                 \
return val;                                                             \
}
#define GFLOATV_Impl(functionName, value) float& functionName() {       \
static float val = value;                                               \
return val;                                                             \
}
#define GCHAR_PTR_Impl(functionName) char*& functionName() {            \
static char* val = nullptr;                                             \
return val;                                                             \
}
#define GCHAR_PTRS_Impl(functionName, size) char*& functionName() {     \
static char* val = (char*)malloc(size + 1);                                    \
return val;                                                             \
}

#define GVAR_Create(type, functionName) type functionName()

#define GVAR_Decl(type, functionName) type& functionName()
#define GBOOL_Decl(functionName) bool& functionName()
#define GINT_Decl(functionName) int& functionName()
#define GFLOAT_Decl(functionName) float& functionName()
#define GCHAR_PTR_Decl(functionName) char*& functionName()

}