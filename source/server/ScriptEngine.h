#ifdef WITH_ANGELSCRIPT
#ifndef SCRIPTENGINE_H__
#define SCRIPTENGINE_H__

#include <string>
#include "angelscript.h"
#include "rornet.h"
#include "scriptmath3d/scriptmath3d.h" // angelscript addon

class ExampleFrameListener;
class GameScript;
class Beam;
class Sequencer;

class ScriptEngine
{
public:
	ScriptEngine(Sequencer *seq);
	~ScriptEngine();

	int loadScript(std::string scriptName);
	void executeString(std::string command);

    void playerDeleted(int uid, int crash);
    void playerAdded(int uid);
    int playerChat(int uid, char *msg);
    int framestep(float dt);

	void timerLoop();

	asIScriptEngine *getEngine() { return engine; };

protected:
    Sequencer *seq;
    asIScriptEngine *engine;                //!< instance of the scripting engine
	asIScriptContext *context;              //!< context in which all scripting happens
	int frameStepFunctionPtr;               //!< script function pointer to the frameStep function
	int playerDeletedFunctionPtr, playerAddedFunctionPtr, playerChatFunctionPtr;
	bool exit;
    pthread_t timer_thread;

	/**
	 * This function initialzies the engine and registeres all types
	 */
    void init();
 
	/**
	 * This is the callback function that gets called when script error occur.
	 * When the script crashes, this function will provide you with more detail
	 * @param msg arguments that contain details about the crash
	 * @param param unkown?
	 */
    void msgCallback(const asSMessageInfo *msg);

	/**
	 * This function reads a file into the provided string.
	 * @param filename filename of the file that should be loaded into the script string
	 * @param script reference to a string where the contents of the file is written to
	 * @return 0 on success, everything else on error
	 */
	int loadScriptFile(const char *fileName, std::string &script);

	// undocumented debugging functions below, not working.
	void ExceptionCallback(asIScriptContext *ctx, void *param);
	void PrintVariables(asIScriptContext *ctx, int stackLevel);
	void LineCallback(asIScriptContext *ctx, void *param);
};

class ServerScript
{
protected:
	ScriptEngine *mse;              //!< local script engine pointer, used as proxy mostly
	Sequencer *seq;     //!< local pointer to the main ExampleFrameListener, used as proxy mostly

public:
	ServerScript(ScriptEngine *se, Sequencer *seq);
	~ServerScript();

	void log(std::string &msg);
	void say(std::string &msg, int uid=-1, int type=0);
	void kick(int kuid, std::string &msg);
	void ban(int kuid, std::string &msg);
	bool unban(int kuid);
	int playerChat(int uid, char *str);
	
	std::string getServerTerrain();

	int sendGameCommand(int uid, std::string cmd);
	
	std::string getUserName(int uid);
	std::string getUserAuth(int uid);
	int getUserPosition(int uid, Vector3 &v);
	int getNumClients();

	void addRef() {};
	void releaseRef() {};
};

#endif //SCRIPTENGINE_H__
#endif //WITH_ANGELSCRIPT