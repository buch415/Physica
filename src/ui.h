//User interface functions header

//Files
string themesFile = "data/cfg/ui/themes.cfg";//Themes file path
string graphicsFile = "data/cfg/graphics.cfg";//Global graphics file

string hudFile = "data/cfg/ui/hud.cfg";//Hud file path
string menuFile = "data/cfg/ui/menu.cfg";//Main menu file path
string levelSelectFile = "data/cfg/ui/levels.cfg";//Level selection filepath
string pauseFile = "data/cfg/ui/pause.cfg";//Pause filepath
string successFile = "data/cfg/ui/success.cfg";//Success file path
string settingsUiFile = "data/cfg/ui/settings.cfg";//Settings window file path
string creditsFile = "data/cfg/ui/credits.cfg";//Credits window file path

//Global graphics
image starOn, starOff;//Star images

//User interface
window hud;//Hud window
control *btnPause, *btnRestart;//Hud buttons
control *labDeaths, *labTime;//Hud labels

window menu;//Menu window
panel *menuFrame;//Menu frame
control *btnPlay, *btnEditor, *btnSettings, *btnCredits, *btnQuit;//Menu buttons

window levelSelect;//Level select window
control levelButton;//Level button
int levelSelect_spacing = 16;//Level selection spacing
int levelSelect_w = 4;//Level selection grid width

window pause;//Pause window
panel* pauseFrame;//Pause frame
control *btnResume, *btnBack;//Pause screen buttons

window success;//Success window
panel* successFrame;//Success frame
control *btnNext, *btnSuccessBack;//Success window buttons
control *labSuccessTime, *labSuccessDeaths;//Success timer and deaths count
control *ratingA, *ratingB, *ratingC;//Rating stars

window settings;//Settings window
panel* settingsFrame;//Settings frame
checkBox *setFullscreen, *setCamFollow, *setSound;//Settings check boxes

window credits;//Credits window
control* creditsLabel;//Credits label

//UI mode enumeration
enum uiMode {
	ui_mainMenu,//Main menu
	
	ui_levels,//Level selection
	ui_game,//Game view
	ui_paused,//Paused game
	ui_success,//Level completion
	
	ui_settings,//Settings window
	
	ui_credits//Credits
} curUiMode = ui_mainMenu;//Current UI mode

//Function to convert a time in msec into a string
string timeToString(int t){
	int t_min = floor (t / 1000 / 60);//Minutes
	int t_sec = int(floor (t / 1000)) % 60;//Seconds
	int t_hun = int(floor (t / 10)) % 100;//Hundreths of second
	
	return (t_min < 10 ? "0" : "") + toString(t_min) + ":" + (t_sec < 10 ? "0" : "") + toString(t_sec) + ":" + (t_hun < 10 ? "0" : "") + toString(t_hun);//Sets timer
}

//Function to redraw level selection window
void redrawLevelSelect(){
	levelSelect.clear();//Clears level selection window
	
	int rows = ceil (current.levels.size() / levelSelect_w);//Rows needed
	int lsH = rows * levelButton.area.h + (rows - 1) * levelSelect_spacing;//Selector height
	int offsetY = (video_h - lsH) / 2;//Y offset
	
	int i;//Counter
	for (i = 0; i <= rows; i++){//For each row
		int n;//Counter
		int rowSize = current.levels.size() - i * levelSelect_w > levelSelect_w ? levelSelect_w : current.levels.size() - i * levelSelect_w;//Elements in row
		int rowW = rowSize * levelButton.area.w + (rowSize - 1) * levelSelect_spacing;//Row width
		int rowOffsetX = (video_w - rowW) / 2;//Row x offset
		
		for (n = 0; n < rowSize; n++){//For each element of the row
			control* c = new control;//New control
			*c = levelButton;//Sets control
			
			c->id = toString(i * levelSelect_w + n);//Sets id
			c->content.t = toString(i * levelSelect_w + n + 1);//Sets text
			
			c->area.x = rowOffsetX + n * (c->area.w + levelSelect_spacing);//Sets x
			c->area.y = offsetY + i * (c->area.h + levelSelect_spacing);//Sets y
			
			levelSelect.push_back(c);//Adds to controls
		}
	}
}

//Function to handle pause click
void pauseClick(clickEventData data){
	current.paused = true;
	curUiMode = ui_paused;
}

//Function to handler reset click
void restartClick(clickEventData data){
	current.reset();
}

//Function to update hud with current game info
void updateHud(){
	labTime->content.t = timeToString(current.time);//Sets timer
	labDeaths->content.t = (current.deaths < 10 ? "0" : "") + toString(current.deaths);//Sets deaths counter
}

//Function to handle play click
void playClick(clickEventData data){
	curUiMode = ui_levels;//Goes to level selector
	PLAYSOUND(clickSfx);//Plays sound
	
	redrawLevelSelect();//Draws level selection window
}

//Function to handle editor click
void editorClick(clickEventData data){
	editing = true;//Sets editing flag
	editorLoop();//Starts editor loop
}

//Function to handle quit click
void quitClick(clickEventData data){
	running = false;
	PLAYSOUND(clickSfx);//Plays sound
}

//Function to handle level click
void levelClick(clickEventData data){
	current.setup(atoi(data.caller->id.c_str()));//Sets up level
	curUiMode = ui_game;//Starts game
	PLAYSOUND(clickSfx);//Plays sound
}

//Function to handle resume click
void resumeClick(clickEventData data){
	curUiMode = ui_game;
	PLAYSOUND(clickSfx);//Plays sound
}

//Function to handle back click
void backClick(clickEventData data){
	curUiMode = ui_levels;
	PLAYSOUND(clickSfx);//Plays sound
}

//Function to show success window
void showSuccess(){
	current.paused = true;//Pauses game
	curUiMode = ui_success;//Shows success window
	
	PLAYSOUND(successSfx);//Plays sound
}

//Function to update success window
void updateSuccess(){
	labSuccessTime->content.t = timeToString(current.time);//Sets time label
	labSuccessDeaths->content.t = (current.deaths < 10 ? "0" : "") + toString(current.deaths);//Sets deaths counter
	
	int rating = current.rating();//Rating
	
	//Sets stars
	ratingA->content.i = rating >= 1 ? starOn : starOff;
	ratingB->content.i = rating >= 2 ? starOn : starOff;
	ratingC->content.i = rating >= 3 ? starOn : starOff;
}

//Function to handle next click
void nextClick(clickEventData data){
	if (current.levelIndex < current.levels.size() - 1){//If not on last level
		current.next();//Jumps to next level
		curUiMode = ui_game;//Sets game mode
		PLAYSOUND(clickSfx);//Plays sound
	}
	
	else backClick({});//Else goes back
}

//Function to handle credits click
void creditsClick(clickEventData data){
	curUiMode = ui_credits;
	PLAYSOUND(clickSfx);
}

//Standard click function (just plays sound)
void stdClick(clickEventData data){
	PLAYSOUND(clickSfx);
}

//Function to show settings window
void showSettings(clickEventData data){
	curUiMode = ui_settings;//Settings mode
	
	//Sets window content
	setFullscreen->checked = fullscreen;
	setCamFollow->checked = camFollow;
	setSound->checked = enableSfx;
	
	PLAYSOUND(clickSfx);//Plays sound
}

//Function to resize video
void resize(int newW, int newH, bool fs){
	if (fs){//If setting fullscreen
		videoWin_w = video_w;//Saves video width in windowed mode
		videoWin_h = video_h;//Saves video height in windowed mode
	}
	
	else {//Else
		videoWin_w = newW;//Sets new windowed width
		videoWin_h = newH;//Sets new windowed height
	}
	
	//Sets size
	video_w = newW;
	video_h = newH;
	
	fullscreen = fs;//Sets fullscreen
	
	if (fullscreen){//If in fullscreen mode
		SDL_Rect best = *(SDL_ListModes(NULL, SDL_SWSURFACE | SDL_FULLSCREEN)[0]);//Best video mode
		
		//Gets video size
		video_w = best.w;
		video_h = best.h;
	}
	
	video = SDL_SetVideoMode(video_w, video_h, 32, SDL_SWSURFACE | (fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE));//Creates video surface
	
	if (menuFrame){//If menu frame is available
		menuFrame->area.x = (video_w - menuFrame->area.w) / 2;//Re-centers on x
		menuFrame->area.y = (video_h - menuFrame->area.h) / 2;//Re-centers on y
	}
	
	if (pauseFrame){//If pause frame is available
		pauseFrame->area.x = (video_w - pauseFrame->area.w) / 2;//Re-centers on x
		pauseFrame->area.y = (video_h - pauseFrame->area.h) / 2;//Re-centers on y
	}
	
	if (successFrame){//If successFrame frame is available
		successFrame->area.x = (video_w - successFrame->area.w) / 2;//Re-centers on x
		successFrame->area.y = (video_h - successFrame->area.h) / 2;//Re-centers on y
	}
	
	if (settingsFrame){//If settingsFrame is available
		settingsFrame->area.x = (video_w - settingsFrame->area.w) / 2;//Centers settings on x
		settingsFrame->area.y = (video_h - settingsFrame->area.h) / 2;//Centers settings on y
	}
	
	redrawLevelSelect();//Redraws level selection window
}

//Function to apply settings
void applySettings(){
	if (setFullscreen->checked != fullscreen) resize(videoWin_w, videoWin_h, setFullscreen->checked);//Applies fullscreen
	camFollow = setCamFollow->checked;//Applies cam follow
	enableSfx = setSound->checked;//Applies sound settings
}

//UI loading and setup function
void loadUI(){
	loadThemesDB(themesFile);//Loads themes
	
	hud = loadWindow(hudFile, "hud");//Loads hud
	btnPause = hud.getControl("pause");//Gets pause button
	btnRestart = hud.getControl("restart");//Gets restart button
	labDeaths = hud.getControl("deaths");//Gets deaths counter
	labTime = hud.getControl("timer");//Gets timer
	
	btnPause->release.handlers.push_back(pauseClick);//Adds click handler to pause
	btnRestart->release.handlers.push_back(restartClick);//Adds click handler to restart
	
	menu = loadWindow(menuFile, "menu");//Loads menu
	menuFrame = (panel*) menu.getControl("frame");//Gets frame
	btnPlay = menu.getControl("frame.play");//Gets play button
	btnEditor = menu.getControl("frame.editor");//Gets editor button
	btnSettings = menu.getControl("frame.settings");//Gets settings button
	btnCredits = menu.getControl("frame.credits");//Gets credits button
	btnQuit = menu.getControl("frame.quit");//Gets quit button
	
	menuFrame->area.x = (video_w - menuFrame->area.w) / 2;//Centers menu on x
	menuFrame->area.y = (video_h - menuFrame->area.h) / 2;//Centers menu on y
	
	btnPlay->release.handlers.push_back(playClick);//Adds click handler to play
	btnEditor->release.handlers.push_back(editorClick);//Adds click handler to editor
	btnSettings->release.handlers.push_back(showSettings);//Adds click handler to settings
	btnCredits->release.handlers.push_back(creditsClick);//Adds click handler to credits
	btnQuit->release.handlers.push_back(quitClick);//Adds click handler to quit
	
	levelSelect = loadWindow(levelSelectFile, "levels");//Loads level selection window
	levelButton = *levelSelect.getControl("levelButton");//Sets default level button
	levelButton.release.handlers.push_back(levelClick);//Adds click handler to level button
	
	redrawLevelSelect();//Draws level selection
	
	pause = loadWindow(pauseFile, "pause");//Loads pause window
	pauseFrame = (panel*) pause.getControl("frame");//Gets frame panel
	btnResume = pause.getControl("frame.resume");//Gets resume button
	btnBack = pause.getControl("frame.back");//Gets back button
	
	pauseFrame->area.x = (video_w - pauseFrame->area.w) / 2;//Centers pause on x
	pauseFrame->area.y = (video_h - pauseFrame->area.h) / 2;//Centers pause on y
	
	btnResume->release.handlers.push_back(resumeClick);//Adds resume click handler
	btnBack->release.handlers.push_back(backClick);//Adds back click handler
	
	success = loadWindow(successFile, "success");//Loads success window
	successFrame = (panel*) success.getControl("frame");//Gets frame
	btnNext = success.getControl("frame.next");//Gets next button
	btnSuccessBack = success.getControl("frame.back");//Gets back button
	labSuccessTime = success.getControl("frame.time");//Gets time label
	labSuccessDeaths = success.getControl("frame.deaths");//Gets deaths label
	ratingA = success.getControl("frame.ratingA");//Gets first star
	ratingB = success.getControl("frame.ratingB");//Gets second star
	ratingC = success.getControl("frame.ratingC");//Gets third star
	
	successFrame->area.x = (video_w - successFrame->area.w) / 2;//Centers success on x
	successFrame->area.y = (video_h - successFrame->area.h) / 2;//Centers success on y
	
	btnSuccessBack->release.handlers.push_back(backClick);//Adds back click handler
	btnNext->release.handlers.push_back(nextClick);//Adds next click handler
	
	settings = loadWindow(settingsUiFile, "settings");//Loads settings file
	settingsFrame = (panel*) settings.getControl("frame");//Gets frame
	setFullscreen = (checkBox*) settings.getControl("frame.fullscreen");//Gets fullscreen checkbox
	setCamFollow = (checkBox*) settings.getControl("frame.camFollow");//Gets camera follow checkbox
	setSound = (checkBox*) settings.getControl("frame.enableSfx");
	
	settingsFrame->area.x = (video_w - settingsFrame->area.w) / 2;//Centers settings on x
	settingsFrame->area.y = (video_h - settingsFrame->area.h) / 2;//Centers settings on y
	
	setFullscreen->release.handlers.push_back(stdClick);//Adds click handler
	setCamFollow->release.handlers.push_back(stdClick);//Adds click handler
	setSound->release.handlers.push_back(stdClick);//Adds click handler
	
	credits = loadWindow(creditsFile, "credits");//Loads credits file
	creditsLabel = credits.getControl("credits");//Gets credits label
	
	creditsLabel->area.x = (video_w - creditsLabel->area.w) / 2;//Centers credits on x
	creditsLabel->area.y = (video_h - creditsLabel->area.h) / 2;//Centers credits on y
}

//Graphics info file loading function
void loadGraphics(){
	fileData f (graphicsFile);//Source file
	object g = f.objGen("graphics");//Generates object
	
	//Gets data
	object* o_starOn = get <object> (&g.o, "starOn");
	object* o_starOff = get <object> (&g.o, "starOff");
	object* o_handle = get <object> (&g.o, "handle");
	
	if (o_starOn) starOn.fromScriptObj(*o_starOn);
	if (o_starOff) starOff.fromScriptObj(*o_starOff);
	if (o_handle) handle.fromScriptObj(*o_handle);
}