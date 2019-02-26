#ifndef __XML_NODE_INL__
#define __XML_NODE_INL__
#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


#undef NEW
#undef NEW_ARGS
#endif//__XML_NODE_INL__