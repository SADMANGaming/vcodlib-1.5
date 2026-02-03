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

void gsc_utils_stringtoistring()
{
    const char* string;

    if ( !stackGetParams("s", &string) )
	{
		stackError("gsc_utils_addistring() one argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

    stackPushIString(string);
}

void gsc_utils_exec()
{
    const char* text;

    if ( !stackGetParams("s", &text) )
	{
		stackError("gsc_utils_exec() one argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

    Cbuf_ExecuteText(EXEC_APPEND, text);
    
    stackPushBool(qtrue);
}

void gsc_utils_tolower() // From cod2rev
{
    char c;
    int i;
    const char *string;
    char tempString[MAX_STRINGLENGTH];

    if (!stackGetParams("s", &string))
    {
        stackError("gsc_utils_tolower() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    for (i = 0; i < MAX_STRINGLENGTH; ++i)
    {
        c = tolower(*string);
        tempString[i] = c;

        if (!c)
        {
            stackPushString(tempString);
            return;
        }

        ++string;
    }

    stackError("gsc_utils_tolower() string too long");
}

void gsc_utils_getsystemtime()
{
    time_t timer;
    stackPushInt(time(&timer));
}

void gsc_utils_logprintconsole()
{
    char *str;

    if (!stackGetParams("s", &str))
    {
        stackError("gsc_utils_logprintconsole() argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if (!strlen(str) || strlen(str) > MAX_STRINGLENGTH)
    {
        stackError("gsc_utils_logprintconsole() invalid string length");
        stackPushUndefined();
        return;
    }

    Com_Printf("%s", str);

    stackPushBool(qtrue);
}

void gsc_utils_colorhex()
{
    char *hex;
    vec3_t colour;
    char buf[3];
    char *end;

    if ( !stackGetParams("s", &hex) )
    {
        stackError("gsc_utils_colorhex() one argument is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if ( strlen(hex) != 7 || hex[0] != '#' )
    {
        stackError("gsc_utils_colorhex() invalid hex format, expected #RRGGBB");
        stackPushUndefined();
        return;
    }

    buf[0] = hex[1];
    buf[1] = hex[2];
    buf[2] = '\0';
    int r = strtol(buf, &end, 16);

    buf[0] = hex[3];
    buf[1] = hex[4];
    int g = strtol(buf, &end, 16);

    buf[0] = hex[5];
    buf[1] = hex[6];
    int b = strtol(buf, &end, 16);

    if ( r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 )
    {
        stackError("gsc_utils_colorhex() hex value out of range");
        stackPushUndefined();
        return;
    }

    colour[0] = (float)r / 255.0f;
    colour[1] = (float)g / 255.0f;
    colour[2] = (float)b / 255.0f;

    stackPushVector(colour);
}

void gsc_utils_colorRainbow()
{
    static int phase = 0;   // 0–1535
    vec3_t colour;

    int p = phase;

    if (p < 256) {
        colour[0] = 1.0f;
        colour[1] = p / 255.0f;
        colour[2] = 0.0f;
    }
    else if (p < 512) {
        colour[0] = (512 - p) / 255.0f;
        colour[1] = 1.0f;
        colour[2] = 0.0f;
    }
    else if (p < 768) {
        colour[0] = 0.0f;
        colour[1] = 1.0f;
        colour[2] = (p - 512) / 255.0f;
    }
    else if (p < 1024) {
        colour[0] = 0.0f;
        colour[1] = (1024 - p) / 255.0f;
        colour[2] = 1.0f;
    }
    else if (p < 1280) {
        colour[0] = (p - 1024) / 255.0f;
        colour[1] = 0.0f;
        colour[2] = 1.0f;
    }
    else {
        colour[0] = 1.0f;
        colour[1] = 0.0f;
        colour[2] = (1536 - p) / 255.0f;
    }

    phase = (phase + 1) % 1536;

    stackPushVector(colour);
}

void gsc_utils_colorrgb()
{
    int r, g, b;
    vec3_t colour;

    if ( !stackGetParams("iii", &r, &g, &b) )
    {
        stackError("gsc_utils_colorrgb() one or more arguments is undefined or has a wrong type");
        stackPushUndefined();
        return;
    }

    if ( r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 )
    {
        stackError("gsc_utils_colorrgb() RGB values must be between 0 and 255");
        stackPushUndefined();
        return;
    }

    colour[0] = (float)r / 255.0f;
    colour[1] = (float)g / 255.0f;
    colour[2] = (float)b / 255.0f;

    stackPushVector(colour);
}
