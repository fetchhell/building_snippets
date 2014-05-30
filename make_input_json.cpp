#include <stdio.h>
#include <stdlib.h>

int main()
{
   const int sz = 1000000;
   char buf[sz];

   FILE *input = fopen("json_input.txt", "r");
   fgets(buf, sz, input); 
   fclose(input);

   FILE *output = fopen("json_input", "w");
   fprintf(output, "%s", buf);
   fclose(output);

   return 0;
}
