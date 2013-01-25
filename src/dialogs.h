//Physica - dialogs

//Message box
class msgbox: public dialog {
	public:
	panel answerButton;//Answer button
	control* text;//Text control
	
	int ansSpacing;//Answer spacing
	int margin;//Margin
	
	//Constructor
	msgbox() {
		dialogFrame = NULL;
		
		frameBegin = NULL;
		frameEnd = NULL;
		events = NULL;
		quitFlag = NULL;
		
		text = NULL;
		
		ansSpacing = 4;
		margin = 16;
	};
	
	//Load function
	void loadDialog(string path){
		dialog::loadDialog(path);
		
		panel* p = (panel*) dialogWindow.getControl("frame.answer");//Gets answer
		answerButton = *p;//Sets answer button
		
		clearAns();//Removes unused buttons
		
		text = dialogWindow.getControl("frame.text");//Gets text
	}
	
	//Function to clear answer buttons
	void clearAns(){
		list<control*>::iterator i;
		
		for (i = dialogFrame->children.begin(); i != dialogFrame->children.end(); i++){//For each child
			if ((*i)->id != "text"){//If not text
				delete *i;//Deletes
				i = dialogFrame->children.erase(i);//Erases from list
				i--;//Goes back
			}
		}
	}
	
	//Show function
	int show(SDL_Surface* target, string prompt, int ansc, string ansv[], bool kp = false){
		text->content.t = prompt;//Sets text
		
		SDL_Surface* old = SDL_CreateRGBSurface(SDL_SWSURFACE, target->w, target->h, 32, 0, 0, 0, 0);//Surface to store target
		SDL_BlitSurface(target, NULL, old, NULL);//Blits target on surface
		
		int n;//Counter
		int w = ansc * answerButton.area.w + (ansc - 1) * ansSpacing;//Total width
		
		if (dialogFrame->area.w < w + 2 * margin){//If message is too small
			dialogFrame->area.w = w + 2 * margin;//Sets frame width
			answerButton.area.x = margin;//Sets first button x coord
		}
		
		else {//Else
			answerButton.area.x = (dialogFrame->area.w - answerButton.area.w) / 2 - (ansc % 2 ? 0 : answerButton.area.w / 2);//Centers answers
		}
		
		centre(target->w, target->h);//Centers on target
	
		clearAns();//Clears answers
	
		for (n = 0; n < ansc; n++){//For each answer
			panel* p = new panel (answerButton);//New answer button
			
			p->area.x += n * p->area.w + ansSpacing;//Sets x
			p->content.t = ansv[n];//Sets content
			p->id = toString(n);//Sets id
			
			dialogFrame->children.push_back(p);//Adds to frame
		}
		
		SDL_Event e;//Event
		
		while (!quitFlag || *quitFlag){//While running
			if (frameBegin) frameBegin();//Frame beginnig
			
			while (SDL_PollEvent(&e)){//While there are events on stack
				if (events) events(e);//Checks events
				
				dialogWindow.checkEvents(e);//Checks dialog events
				
				list<control*>::iterator i;//Control iterator
				for (i = dialogFrame->children.begin(); i != dialogFrame->children.end(); i++)//For each control
					if ((*i)->id != "text" && (*i)->status == control::pressed) return atoi((*i)->id.c_str());//Returns index if pressed
					
				if (e.type == SDL_KEYDOWN && kp) return -1;//Returns -1 on key press
			}
			
			SDL_BlitSurface(old, NULL, target, NULL);//Prints target
			dialogWindow.print(target);//Prints dialog
			SDL_Flip(target);//Updates target
			
			if (frameEnd) frameEnd();//Frame ending
		}
		
		return -1;//Returns -1 if didn't return any answer
	}
} msgBox;

string msgBox_ans_ok[] = { "OK" };//OK msgbox answer
string msgBox_ans_okCancel[] = { "OK", "Cancel"};//OK/Cancel msgbox answer
string msgBox_ans_yn[] = { "Yes", "No"};//Yes/No msgbox answer

//Input box
class inputbox: public dialog {
	public:
	control* text;//Text control
	inputBox* field;//Field control

	//Constructor
	inputbox(){
		dialogFrame = NULL;
		
		frameBegin = NULL;
		frameEnd = NULL;
		events = NULL;
		quitFlag = NULL;
		
		text = NULL;
	}
	
	//Load function
	void loadDialog(string path){
		dialog::loadDialog(path);//Loads base dialog
		
		text = dialogWindow.getControl("frame.text");//Gets dialog
		field = (inputBox*) dialogWindow.getControl("frame.field");//Gets field
	}
	
	//Show function
	string show(SDL_Surface* target, string prompt){
		text->content.t = prompt;//Sets text
		
		SDL_Surface* old = SDL_CreateRGBSurface(SDL_SWSURFACE, target->w, target->h, 32, 0, 0, 0, 0);//Surface to store target
		SDL_BlitSurface(target, NULL, old, NULL);//Blits target on surface
		
		SDL_Event e;//Event
		
		centre(target->w, target->h);//Centres
		
		while (!quitFlag || *quitFlag){//While running
			if (frameBegin) frameBegin();//Frame beginnig
			
			while (SDL_PollEvent(&e)){//While there are events on stack
				if (events) events(e);//Checks events
				
				dialogWindow.checkEvents(e);//Checks dialog events
				
				if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) return field->content.t;//Returns input field on enter press
				else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return "";//Returns empty on esc
			}
			
			SDL_BlitSurface(old, NULL, target, NULL);//Prints target
			dialogWindow.print(target);//Prints dialog
			SDL_Flip(target);//Updates target
			
			if (frameEnd) frameEnd();//Frame ending
		}
		
		return "";//Returns "" if didn't return any answer
	}
} inputBox;