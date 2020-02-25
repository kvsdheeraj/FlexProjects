
#include <cstdio>
#include "fileSystemUtil.h"

#define VERIFY_USAGE_SHELL_CMD "df | head -2 | tail -1 | sed -r 's/( )+/:/g' | cut -d \":\" -f 5 | sed -r 's/%//g'"
int getFilesystemUsage()
{
    const char *shellcmd = VERIFY_USAGE_SHELL_CMD;
    FILE *fp = NULL;
    int retValue = 0;
    
    fp = popen( shellcmd, "r" );
    if(fp != NULL)
    {
        fscanf(fp, "%d", &retValue);
        pclose(fp);
        //fprintf(
        //    stdout,
        //    "FILE SYSTEM USAGE=%d%%\n", 
        //    retValue );
    }    
    else
    {
        fprintf(
            stdout,
            "Invalid Command <%s>\n", 
            VERIFY_USAGE_SHELL_CMD );
    }
 
    return retValue;
}
