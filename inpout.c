#include "inpout.h"

/***************************************************************************
 * LUA embedding reference
 *
 * http://www.codegurus.be/Programming/luaembedding_en.htm
 *
 ***************************************************************************/

/* exported inpout DLL entries */
lpOut32 gfpOut32;
lpInp32 gfpInp32;
lpIsInpOutDriverOpen gfpIsInpOutDriverOpen;
lpIsXP64Bit gfpIsXP64Bit;


/* glue functions prototypes */
static int outpb_glue (lua_State* L);
static int inpb_glue  (lua_State* L);
static int outpw_glue (lua_State* L);
static int inpw_glue  (lua_State* L);
static int outpdw_glue(lua_State* L);
static int inpdw_glue (lua_State* L);


/* module installler */
void InstallInpOut32(lua_State* L)
{
    HINSTANCE hInpOutDll;
    hInpOutDll = LoadLibrary(L"inpout32.dll");
    if (hInpOutDll != NULL)
    {
        gfpOut32              = (lpOut32             )GetProcAddress(hInpOutDll, "Out32");
        gfpInp32              = (lpInp32             )GetProcAddress(hInpOutDll, "Inp32");
        gfpIsInpOutDriverOpen = (lpIsInpOutDriverOpen)GetProcAddress(hInpOutDll, "IsInpOutDriverOpen");
        gfpIsXP64Bit          = (lpIsXP64Bit         )GetProcAddress(hInpOutDll, "IsXP64Bit");

        if (gfpIsInpOutDriverOpen())
        {
            lua_register(L, "outpb" , outpb_glue);
            lua_register(L, "inpb"  , inpb_glue);
            lua_register(L, "outpw" , outpw_glue);
            lua_register(L, "inpw"  , inpw_glue);
            lua_register(L, "outpdw", outpdw_glue);
            lua_register(L, "inpdw" , inpdw_glue);
        }
    }
    else
    {
        printf("Warning: Could not load InpOut32 DLL. I/O functions are not available.\r\n");
    }
}


 // --------------------------------------------------------------------
 // BYTE OPERATION : OUT
 // --------------------------------------------------------------------
void outpb_lua(unsigned int port, unsigned char data)
{
#if DEBUG_PRINT
    printf("DEBUG: OUTPB PORT=0x%04x DATA=0x%08x\r\n", port, data);
#endif
    gfpOut32(port, data & 0xFF);
}

static int outpb_glue(lua_State* L)
{
    unsigned int port = 0;
    unsigned char data = 0;

    /* check number of arguments */
    int n = lua_gettop(L);
    if (n != 2)
    {
        if (n < 2)
            lua_pushstring(L, "outpb(): not enough arguments");
        else
            lua_pushstring(L, "outpb(): too many arguments");
        lua_error(L);
    }

    /* check argument 1 - PORT */
    if (lua_type(L, 1) == LUA_TNUMBER)
    {
        port = lua_tointeger(L, 1);
    }
    else {
        lua_pushstring(L, "outpb(): parameter 1 is not a number");
        lua_error(L);
    }

    /* check argument 2 - DATA */
    if (lua_type(L, 2) == LUA_TNUMBER) {
        data = lua_tointeger(L, 2);
    }
    else {
        lua_pushstring(L, "outpb(): parameter 2 is not a number");
        lua_error(L);
    }

    outpb_lua(port, data);
    return 0;   // doesn't return values
}


// --------------------------------------------------------------------
// BYTE OPERATION : IN
// --------------------------------------------------------------------

unsigned char inpb_lua(unsigned int port)
{
    unsigned char retval = gfpInp32(port);
#if DEBUG_PRINT
    printf("DEBUG: INPB PORT=0x%04x DATA=0x%08x\r\n", port, retval);
#endif
    return(retval);
}

static int inpb_glue(lua_State* L)
{
    unsigned int port = 0;

    /* check number of arguments */
    int n = lua_gettop(L);
    if (n != 1)
    {
        if (n < 1)
            lua_pushstring(L, "inpb(): not enough arguments");
        else
            lua_pushstring(L, "inpb(): too many arguments");
        lua_error(L);
    }

    /* check argument 1 - PORT */
    if (lua_type(L, 1) == LUA_TNUMBER)
    {
        port = lua_tointeger(L, 1);
    }
    else {
        lua_pushstring(L, "inpb(): parameter 1 is not a number");
        lua_error(L);
    }

    lua_pushinteger(L, inpb_lua(port));

    return 1; // returns single value
}


// --------------------------------------------------------------------
 // WORD OPERATION : OUT
 // --------------------------------------------------------------------
void outpw_lua(unsigned int port, unsigned int data)
{
#if DEBUG_PRINT
    printf("DEBUG: OUTPW PORT=0x%04x DATA=0x%08x\r\n", port, data);
#endif
    gfpOut32(port+0, (data >> 0) & 0xFF);
    gfpOut32(port+1, (data >> 8) & 0xFF);
}

static int outpw_glue(lua_State* L)
{
    unsigned int port = 0;
    unsigned int data = 0;

    /* check number of arguments */
    int n = lua_gettop(L);
    if (n != 2)
    {
        if (n < 2)
            lua_pushstring(L, "outpw(): not enough arguments");
        else
            lua_pushstring(L, "outpw(): too many arguments");
        lua_error(L);
    }

    /* check argument 1 - PORT */
    if (lua_type(L, 1) == LUA_TNUMBER)
    {
        port = lua_tointeger(L, 1);
    }
    else {
        lua_pushstring(L, "outpw(): parameter 1 is not a number");
        lua_error(L);
    }

    /* check argument 2 - DATA */
    if (lua_type(L, 2) == LUA_TNUMBER) {
        data = lua_tointeger(L, 2);
    }
    else {
        lua_pushstring(L, "outpw(): parameter 2 is not a number");
        lua_error(L);
    }

    outpw_lua(port, data);
    return 0;   // doesn't return values
}


// --------------------------------------------------------------------
// WORD OPERATION : IN
// --------------------------------------------------------------------

unsigned int inpw_lua(unsigned int port)
{
    unsigned char byte0 = gfpInp32(port);
    unsigned char byte1 = gfpInp32(port + 1);
    unsigned int retval = (byte1 << 8) + byte0;
#if DEBUG_PRINT
    printf("DEBUG: INPW PORT=0x%04x DATA=0x%08x\r\n", port, retval);
#endif
    return(retval);
}

static int inpw_glue(lua_State* L)
{
    unsigned int port = 0;

    /* check number of arguments */
    int n = lua_gettop(L);
    if (n != 1)
    {
        if (n < 1)
            lua_pushstring(L, "inpw(): not enough arguments");
        else
            lua_pushstring(L, "inpw(): too many arguments");
        lua_error(L);
    }

    /* check argument 1 - PORT */
    if (lua_type(L, 1) == LUA_TNUMBER)
    {
        port = lua_tointeger(L, 1);
    }
    else {
        lua_pushstring(L, "inpw(): parameter 1 is not a number");
        lua_error(L);
    }

    lua_pushinteger(L, inpw_lua(port));

    return 1; // returns single value
}


// --------------------------------------------------------------------
 // DWORD OPERATION : OUT
 // --------------------------------------------------------------------
void outpdw_lua(unsigned int port, unsigned long data)
{
#if DEBUG_PRINT
    printf("DEBUG: OUTPDW PORT=0x%04x DATA=0x%08x\r\n", port, data);
#endif
    gfpOut32(port + 0, (data >> 0) & 0xFF);
    gfpOut32(port + 1, (data >> 8) & 0xFF);
    gfpOut32(port + 2, (data >> 16) & 0xFF);
    gfpOut32(port + 3, (data >> 24) & 0xFF);
}

static int outpdw_glue(lua_State* L)
{
    unsigned int port = 0;
    unsigned long data = 0;

    /* check number of arguments */
    int n = lua_gettop(L);
    if (n != 2)
    {
        if (n < 2)
            lua_pushstring(L, "outpdw(): not enough arguments");
        else
            lua_pushstring(L, "outpdw(): too many arguments");
        lua_error(L);
    }

    /* check argument 1 - PORT */
    if (lua_type(L, 1) == LUA_TNUMBER)
    {
        port = lua_tointeger(L, 1);
    }
    else {
        lua_pushstring(L, "outpdw(): parameter 1 is not a number");
        lua_error(L);
    }

    /* check argument 2 - DATA */
    if (lua_type(L, 2) == LUA_TNUMBER) {
        data = lua_tointeger(L, 2);
    }
    else {
        lua_pushstring(L, "outpdw(): parameter 2 is not a number");
        lua_error(L);
    }

    outpdw_lua(port, data);
    return 0;   // doesn't return values
}


// --------------------------------------------------------------------
// DWORD OPERATION : IN
// --------------------------------------------------------------------

unsigned long inpdw_lua(unsigned int port)
{
    unsigned char byte0 = gfpInp32(port);
    unsigned char byte1 = gfpInp32(port + 1);
    unsigned char byte2 = gfpInp32(port + 2);
    unsigned char byte3 = gfpInp32(port + 3);
    unsigned long retval = (byte3 << 24) + (byte2 << 16) + (byte1 << 8) + byte0;
#if DEBUG_PRINT
    printf("DEBUG: INPDW PORT=0x%04x DATA=0x%08x\r\n", port, retval);
#endif
    return(retval);
}

static int inpdw_glue(lua_State* L)
{
    unsigned int port = 0;

    /* check number of arguments */
    int n = lua_gettop(L);
    if (n != 1)
    {
        if (n < 1)
            lua_pushstring(L, "inpdw(): not enough arguments");
        else
            lua_pushstring(L, "inpdw(): too many arguments");
        lua_error(L);
    }

    /* check argument 1 - PORT */
    if (lua_type(L, 1) == LUA_TNUMBER)
    {
        port = lua_tointeger(L, 1);
    }
    else {
        lua_pushstring(L, "inpdw(): parameter 1 is not a number");
        lua_error(L);
    }

    lua_pushinteger(L, inpdw_lua(port));

    return 1; // returns single value
}