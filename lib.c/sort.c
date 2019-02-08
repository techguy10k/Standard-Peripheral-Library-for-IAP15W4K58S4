#include"sort.h"
#include"stc15.h"

void insert_sort(int *a,int length)
{
  int j,key,i;

  for(j = 1; j < length; ++j)
    {
      key = a[j];
      for(i = j - 1 ; i >= 0; --i)
        {
          if(a[i] < key)
            {
              break;
            }
          a[i+1] = a[i];
        }
      a[i+1] = key;
    }


}
