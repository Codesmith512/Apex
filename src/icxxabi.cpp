#include "icxxabi"

/* STL */
#include <vector>

/**
 * @struct atexit_t
 * @brief the type that holds a single function to be called at-exit
 */
struct atexit_t
{
  void (*dtor_handle)(void*);
  void* obj;
};

/* The vector used for holding our atexit_t's */
std::vector<atexit_t>* handles;

/* The register function */
int __cxa_atexit(void (*dtor)(void*), void* obj, void*)
{
  if(!handles)
    handles = new std::vector<atexit_t>();

  handles->push_back({dtor, obj});
  return 0;
}

/* The cleanup function */
void __cxa_finalize(void* obj)
{
  if(!handles)
    return;

  for(std::size_t i = 0; i < handles->size(); ++i)
    if((*handles)[i].obj == obj || !obj)
    {
      (*((*handles)[i].dtor_handle))((*handles)[i].obj);
      handles->erase(handles->begin() + i);
    }

  if(!obj)
    delete std::exchange(handles, nullptr);
}