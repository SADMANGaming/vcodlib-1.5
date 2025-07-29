#include "gsc_utils.hpp"


//////////// FILE SYSTEM

void gsc_utils_file_exists()
{
    char *filename;

    if ( !stackGetParams("s", &filename) )
    {
        stackError("gsc_utils_file_exists() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    int file_exists = access(filename, F_OK) != -1;
    stackPushInt(file_exists);
}

void gsc_utils_fopen()
{
    FILE *file;
    char *filename, *mode;

    if ( !stackGetParams("ss", &filename, &mode) )
    {
        stackError("gsc_utils_fopen() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    file = fopen(filename, mode);

    if ( !file )
    {
        stackError("gsc_utils_fopen() returned an error");
        stackPushUndefined();
        return;
    }

    stackPushInt((int)file);
}

void gsc_utils_fread()
{
    FILE *file;

    if ( !stackGetParams("i", &file) )
    {
        stackError("gsc_utils_fread() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if ( !file )
    {
        stackError("gsc_utils_fread() returned an error");
        stackPushUndefined();
        return;
    }

    char buffer[256];
    int ret = fread(buffer, 1, 255, file);

    if ( !ret )
    {
        stackPushUndefined();
        return;
    }

    buffer[ret] = '\0';
    stackPushString(buffer);
}

void gsc_utils_fwrite()
{
    FILE *file;
    char *buffer;

    if ( !stackGetParams("is", &file, &buffer) )
    {
        stackError("gsc_utils_fwrite() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if ( !file )
    {
        stackError("gsc_utils_fwrite() returned an error");
        stackPushUndefined();
        return;
    }

    stackPushInt(fwrite(buffer, 1, strlen(buffer), file));
}

void gsc_utils_fclose()
{
    FILE *file;

    if ( !stackGetParams("i", &file) )
    {
        stackError("gsc_utils_fclose() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if ( !file )
    {
        stackError("gsc_utils_fclose() returned an error");
        stackPushUndefined();
        return;
    }

    stackPushInt(fclose(file));
}
//////////// END

void gsc_utils_system()
{
    char *command;
    if ( !stackGetParams("s", &command) )
    {
        stackError("gsc_utils_system() one arguments is undefined or has a wrong type");
        stackPushUndefined();
        return; 
    }

    system(command);
    stackPushBool(qtrue);
}

void gsc_utils_sendcommandtoclient()
{
    int clientNum;
    char *message;

    if ( !stackGetParams("is", &clientNum, &message) )
    {
        stackError("gsc_utils_sendcommandtoclient() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    trap_SendServerCommand(clientNum, SV_CMD_CAN_IGNORE, message);
    stackPushBool(qtrue);
}

void gsc_utils_strtok() // From cod2rev
{
    signed int j;
    char c;
    int dest;
    int i;
    signed int len;
    const char *tok;
    const char *delim;
    char tempString[1024];

    delim = Scr_GetString(0);
    tok = Scr_GetString(1u);
    len = strlen(tok);
    dest = 0;

    Scr_MakeArray();

    for ( i = 0; ; ++i )
    {
        c = delim[i];

        if ( !c )
            break;

        for ( j = 0; j < len; ++j )
        {
            if ( c == tok[j] )
            {
                if ( dest )
                {
                    tempString[dest] = 0;
                    Scr_AddString(tempString);
                    Scr_AddArray();
                    dest = 0;
                }

                goto skip;
            }
        }

        tempString[dest] = c;

        if ( ++dest > 1023 )
            stackError("gsc_utils_strtok() string too long");
skip:
        ;
    }

    if ( dest )
    {
        tempString[dest] = 0;
        Scr_AddString(tempString);
        Scr_AddArray();
    }
}

void gsc_utils_getsubstr()
{
    int end;
    char c;
    int i;
    int source;
    int start;
    const char *string;
    char tempString[1024];

    string = Scr_GetString(0);
    start = Scr_GetInt(1u);

    if ( Scr_GetNumParam() <= 2 )
        end = 0x7FFFFFFF;
    else
        end = Scr_GetInt(2u);

    source = start;

    for ( i = 0; source < end; ++i )
    {
        if ( i > 1023 )
            stackError("gsc_utils_getsubstr() string too long");

        c = string[source];

        if ( !c )
            break;

        tempString[i] = c;
        ++source;
    }

    tempString[i] = 0;
    stackPushString(tempString);
}

void gsc_utils_fromhex()
{
	char *input;

	if ( !stackGetParams("s", &input) )
	{
		stackError("gsc_utils_fromhex() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	int output;

	if ( sscanf(input, "%X", &output) != 1 )
	{
		stackError("gsc_utils_fromhex() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	stackPushInt(output);
}
