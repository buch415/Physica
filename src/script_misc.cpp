/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "script_misc.h"

//Function to load a file into a string
void scReadFile(CScriptVar* c, void * userdata){
	string fName = preprocessFilePath(c->getParameter("file")->getString());
	ifstream i (fName.c_str());
	
	string result = "";
	string tmp;
	
	while (!i.eof()){
		getline(i, tmp);
		result += tmp + "\n";
	}
	
	i.close();
	c->getReturnVar()->setString(result);
}

//Function to import (and execute) a script from a file
void scImportScript(CScriptVar* c, void* userdata){
	string fName = preprocessFilePath(c->getParameter("file")->getString());
	ifstream i (fName.c_str());
	
	string script = "";
	string tmp;
	
	while (!i.eof()){
		getline(i, tmp);
		script += tmp + "\n";
	}
	
	i.close();
	((CTinyJS*) userdata)->execute(script);
}

//Function for output
void scOut(CScriptVar* c, void* userdata){
	CScriptVar* parameter = c->getParameter("out");
	outputLog << "[SCRIPT OUTPUT] " << parameter->getString() << endl;
}

//Function to convert keycode to corresponding text
void scGetKeyName(CScriptVar* c, void* userdata){
	c->getReturnVar()->setString(SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode) c->getParameter("key")->getInt())));
}

//Function to register misc functions
void registerMiscFunctions(CTinyJS* c){
	c->addNative("function readFile (file)", scReadFile, NULL);
	c->addNative("function importScript (file)", scImportScript, c);
	c->addNative("function output (out)", scOut, NULL);
	c->addNative("function getKeyName (key)", scGetKeyName, NULL);
}
