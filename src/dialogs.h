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
		update = NULL;
		
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
	int show(SDL_Surface* target, string prompt, int ansc, string ansv[], bool kp = false, bool dark = true){
		text->content.t = prompt;//Sets text
		
		if (dark) DARK;
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
			p->content.t = getText(ansv[n]);//Sets content
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
					if ((*i)->id != "text" && (*i)->release.triggered) return atoi((*i)->id.c_str());//Returns index if pressed
					
				if (e.type == SDL_KEYDOWN && kp) return -1;//Returns -1 on key press
			}
			
			SDL_BlitSurface(old, NULL, target, NULL);//Prints target
			dialogWindow.print(target);//Prints dialog
			if (update) update();//Updates
			
			if (frameEnd) frameEnd();//Frame ending
		}
		
		return -1;//Returns -1 if didn't return any answer
	}
} msgBox;

string msgBox_ans_ok[] = { "ok" };//OK msgbox answer
string msgBox_ans_okCancel[] = { "ok", "cancel"};//OK/Cancel msgbox answer
string msgBox_ans_yn[] = { "yes", "no"};//Yes/No msgbox answer

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
		field = NULL;
		update = NULL;
	}
	
	//Load function
	void loadDialog(string path){
		dialog::loadDialog(path);//Loads base dialog
		
		text = dialogWindow.getControl("frame.text");//Gets dialog
		field = (inputBox*) dialogWindow.getControl("frame.field");//Gets field
	}
	
	//Show function
	string show(SDL_Surface* target, string prompt, string defaultAns = ""){
		text->content.t = prompt;//Sets text
		
		DARK;
		SDL_Surface* old = SDL_CreateRGBSurface(SDL_SWSURFACE, target->w, target->h, 32, 0, 0, 0, 0);//Surface to store target
		SDL_BlitSurface(target, NULL, old, NULL);//Blits target on surface
		
		SDL_Event e;//Event
		
		centre(target->w, target->h);//Centres
		
		field->content.t = defaultAns;//Sets default answer
		field->edit = true;//Focuses field
		
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
			if (update) update();//Updates
			
			if (frameEnd) frameEnd();//Frame ending
		}
		
		return "";//Returns "" if didn't return any answer
	}
} inputBoxDialog;

//Image dialog
class imgpreview: public dialog {
	public:
	int margin;//Margin
	
	//Constructor
	imgpreview(){
		dialogFrame = NULL;
		
		frameBegin = NULL;
		frameEnd = NULL;
		events = NULL;
		quitFlag = NULL;
		update = NULL;
		
		margin = 16;
	}
	
	//Load function
	void loadDialog(string path){
		dialog::loadDialog(path);//Loads normally
		dialogFrame->content.contentType = CONTENT_IMAGE;//Sets content type
	}
	
	//Show function
	void show(SDL_Surface* target, image i){
		if (!i.valid()) return;//Returns if image isn't valid
		
		dialogFrame->content.i = i;//Sets image
		dialogFrame->area.w = i.w() + 2 * margin;//Sets width
		dialogFrame->area.h = i.h() + 2 * margin;//Sets height
		
		centre(target->w, target->h);//Centers
		
		DARK;
		SDL_Surface* old = SDL_CreateRGBSurface(SDL_SWSURFACE, target->w, target->h, 32, 0, 0, 0, 0);//Surface to store target
		SDL_BlitSurface(target, NULL, old, NULL);//Blits target on surface
		
		SDL_Event e;//Event
		
		while (!quitFlag || *quitFlag){//While running
			if (frameBegin) frameBegin();//Frame beginnig
			
			while (SDL_PollEvent(&e)){//While there are events on stack
				if (events) events(e);//Checks events
				
				dialogWindow.checkEvents(e);//Checks dialog events
				
				if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONUP) return;//Returns on key press
			}
			
			SDL_BlitSurface(old, NULL, target, NULL);//Prints target
			dialogWindow.print(target);//Prints dialog
			if (update) update();//Updates
			
			if (frameEnd) frameEnd();//Frame ending
		}
	}
} imgPreview;

//Image input
class imginput: public dialog {
	public:
	inputBox *idField, *pathField, *rectX, *rectY, *rectW, *rectH;//Input boxes
	control *ok, *preview, *cancel;//Buttons
	
	//Constructor
	imginput(){
		dialogFrame = NULL;
		
		frameBegin = NULL;
		frameEnd = NULL;
		events = NULL;
		quitFlag = NULL;
		update = NULL;
		
		idField = NULL;
		pathField = NULL;
		rectX = NULL;
		rectY = NULL;
		rectW = NULL;
		
		ok = NULL;
		preview = NULL;
		cancel = NULL;
	}
	
	//Load function
	void loadDialog(string path){
		dialog::loadDialog(path);//Loads dialog
		
		//Gets controls
		idField = (inputBox*) dialogWindow.getControl("frame.idField");
		pathField = (inputBox*) dialogWindow.getControl("frame.pathField");
		rectX = (inputBox*) dialogWindow.getControl("frame.rectX");
		rectY = (inputBox*) dialogWindow.getControl("frame.rectY");
		rectW = (inputBox*) dialogWindow.getControl("frame.rectW");
		rectH = (inputBox*) dialogWindow.getControl("frame.rectH");
		
		ok = dialogWindow.getControl("frame.ok");
		preview = dialogWindow.getControl("frame.preview");
		cancel = dialogWindow.getControl("frame.cancel");
	}
	
	//Show function
	image *show(SDL_Surface* target, image *i){
		if (i && i->valid()){//If image's valid
			pathField->content.t = i->imageFilePath;
			rectX->content.t = toString(i->rect.x);
			rectY->content.t = toString(i->rect.y);
			rectW->content.t = toString(i->rect.w);
			rectH->content.t = toString(i->rect.h);
		}
		
		centre(target->w, target->h);//Centers
		
		DARK;
		SDL_Surface* old = SDL_CreateRGBSurface(SDL_SWSURFACE, target->w, target->h, 32, 0, 0, 0, 0);//Surface to store target
		SDL_BlitSurface(target, NULL, old, NULL);//Blits target on surface
		
		SDL_Event e;//Event
		
		while (!quitFlag || *quitFlag){//While running
			if (frameBegin) frameBegin();//Frame beginnig
			
			while (SDL_PollEvent(&e)){//While there are events on stack
				if (events) events(e);//Checks events
				
				dialogWindow.checkEvents(e);//Checks dialog events
				
				if (ok->release.triggered)//If pressed ok
					return new image(
									idField->content.t,
									pathField->content.t,
									atoi(rectX->content.t.c_str()),
									atoi(rectY->content.t.c_str()),
									atoi(rectW->content.t.c_str()),
									atoi(rectH->content.t.c_str()));//Result
										
				if (preview->release.triggered){//If pressed preview
					image i (idField->content.t,
							pathField->content.t,
							atoi(rectX->content.t.c_str()),
							atoi(rectY->content.t.c_str()),
							atoi(rectW->content.t.c_str()),
							atoi(rectH->content.t.c_str()));//Image
							
					imgPreview.show(target, i);//Shows image
				}
										
				if (cancel->release.triggered)//If pressed cancel
					return NULL;//Returns null
			}
			
			SDL_BlitSurface(old, NULL, target, NULL);//Prints target
			dialogWindow.print(target);//Prints dialog
			if (update) update();//Updates
			
			if (frameEnd) frameEnd();//Frame ending
		}
	}
} imgInput;

//Achievement dialog
class achdialog: public dialog {
	public:
	control *icon;//Achievement icon
	control *iconEdit;//Edit icon
	
	inputBox *idField, *nameField, *infoField, *verifyField;//Fields
	checkBox *checkOnce;//Check once flag
	
	control *ok, *cancel;//Ok and cancel buttons
	
	//Constructor
	achdialog(){
		dialogFrame = NULL;
		
		frameBegin = NULL;
		frameEnd = NULL;
		events = NULL;
		quitFlag = NULL;
		update = NULL;
		
		icon = NULL;
		iconEdit = NULL;
		idField = NULL;
		nameField = NULL;
		infoField = NULL;
		verifyField = NULL;
		checkOnce = NULL;
		
		ok = NULL;
		cancel = NULL;
	}
	
	//Load function
	void loadDialog(string path){
		dialog::loadDialog(path);//Loads normally
		
		//Gets controls
		icon = dialogWindow.getControl("frame.icon");
		iconEdit = dialogWindow.getControl("frame.iconEdit");
		idField = (inputBox*) dialogWindow.getControl("frame.idField");
		nameField = (inputBox*) dialogWindow.getControl("frame.nameField");
		infoField = (inputBox*) dialogWindow.getControl("frame.infoField");
		verifyField = (inputBox*) dialogWindow.getControl("frame.verifyField");
		checkOnce = (checkBox*) dialogWindow.getControl("frame.checkOnce");
		ok = dialogWindow.getControl("frame.ok");
		cancel = dialogWindow.getControl("frame.cancel");
		
		icon->content.contentType = CONTENT_IMAGE;//Sets image content type for icon
	}
	
	//Show function
	achievement* show(SDL_Surface* target, achievement *a = NULL){
		if (a){
			icon->content.i = a->icon;//Sets icon
			idField->content.t = a->id;//Sets id
			nameField->content.t = a->name;//Sets name
			infoField->content.t = a->info;//Sets info
			verifyField->content.t = a->verifyExpr.exprToString();//Sets expression
		}
		
		else {
			icon->content.i = *new image;
			idField->content.t = "";
			nameField->content.t = "";
			infoField->content.t = "";
			verifyField->content.t = "";
		}
		
		centre(target->w, target->h);//Centers on screen
		
		DARK;
		SDL_Surface* old = SDL_CreateRGBSurface(SDL_SWSURFACE, target->w, target->h, 32, 0, 0, 0, 0);//Surface to store target
		SDL_BlitSurface(target, NULL, old, NULL);//Blits target on surface
		
		SDL_Event e;//Event
		
		while (!quitFlag || *quitFlag){//While running
			if (frameBegin) frameBegin();//Frame beginnig
			
			while (SDL_PollEvent(&e)){//While there are events on stack
				if (events) events(e);//Checks events
				
				dialogWindow.checkEvents(e);//Checks dialog events
				
				if (ok->release.triggered){//If pressed ok
					if (idField->content.t == ""){//If id is invalid
						msgBox.show(target, getText("idRequired"), 1, msgBox_ans_ok);//Message box
						continue;//Next loop
					}
					
					else {
						achievement *result = new achievement (
									idField->content.t,
									nameField->content.t,
									infoField->content.t,
									verifyField->content.t,
									icon->content.i,
									checkOnce->checked);//Result
						
						if (a) result->langs = a->langs;
						
						return result;
					}
				}
										
				if (iconEdit->release.triggered || icon->release.triggered){//If pressed icon
					image* i = imgInput.show(target, &icon->content.i);//Edits image
					
					if (i) icon->content.i = *i;//Sets icon
				}
										
				if (cancel->release.triggered)//If pressed cancel
					return NULL;//Returns null
			}
			
			SDL_BlitSurface(old, NULL, target, NULL);//Prints target
			dialogWindow.print(target);//Prints dialog
			if (update) update();//Updates
			
			if (frameEnd) frameEnd();//Frame ending
		}
	}
} achDialog;

//Rules dialog
class rulesdialog: public dialog {
	public:
	inputBox *jumpImpulse, *groundSpeed, *groundForce, *groundDamping, *airSpeed, *airForce, *gravity, *jumpCount;//Fields
	control *ok, *cancel;//Ok and cancel buttons
	
	//Constructor
	rulesdialog(){
		dialogFrame = NULL;
		
		frameBegin = NULL;
		frameEnd = NULL;
		events = NULL;
		quitFlag = NULL;
		update = NULL;
		
		jumpImpulse = NULL;
		groundSpeed = NULL;
		groundForce = NULL;
		groundDamping = NULL;
		airSpeed = NULL;
		airForce = NULL;
		gravity = NULL;
		jumpCount = NULL;
		
		ok = NULL;
		cancel = NULL;
	}
	
	//Load function
	void loadDialog(string path){
		dialog::loadDialog(path);//Loads base data
		
		jumpImpulse = (inputBox*) dialogWindow.getControl("frame.jumpImpulse");
		groundSpeed = (inputBox*) dialogWindow.getControl("frame.groundSpeed");
		groundForce = (inputBox*) dialogWindow.getControl("frame.groundForce");
		groundDamping =  (inputBox*) dialogWindow.getControl("frame.groundDamping");
		airSpeed = (inputBox*) dialogWindow.getControl("frame.airSpeed");
		airForce = (inputBox*) dialogWindow.getControl("frame.airForce");
		gravity = (inputBox*) dialogWindow.getControl("frame.gravity");
		jumpCount = (inputBox*) dialogWindow.getControl("frame.jumpCount");
		
		ok = dialogWindow.getControl("frame.ok");
		cancel = dialogWindow.getControl("frame.cancel");
	}
	
	//Show function
	rules* show(SDL_Surface* target, rules* r = NULL){
		if (r){//If rules were specified
			//Sets fields according to rules mask
			if (r->setMask & 0b10000000) jumpImpulse->content.t = toString(r->jumpImpulse);
			else jumpImpulse->content.t = "";
			
			if (r->setMask & 0b01000000) groundSpeed->content.t = toString(r->groundSpeed);
			else groundSpeed->content.t = "";
			
			if (r->setMask & 0b00100000) groundForce->content.t = toString(r->groundForce);
			else groundForce->content.t = "";
			
			if (r->setMask & 0b00010000) groundDamping->content.t = toString(r->groundDamping);
			else groundDamping->content.t = "";
			
			if (r->setMask & 0b00001000) airSpeed->content.t = toString(r->airSpeed);
			else airSpeed->content.t = "";
			
			if (r->setMask & 0b00000100) airForce->content.t = toString(r->airForce);
			else airForce->content.t = "";
			
			if (r->setMask & 0b00000010) gravity->content.t = toString(r->gravity.x) + " " + toString(r->gravity.y);
			else gravity->content.t = "";
			
			if (r->setMask & 0b00000001) jumpCount->content.t = toString(r->jumpCount);
			else jumpCount->content.t = "";
		}
		
		else {//Else
			//Leaves blank
			jumpImpulse->content.t = "";
			groundSpeed->content.t = "";
			groundForce->content.t = "";
			groundDamping->content.t = "";
			airSpeed->content.t = "";
			airForce->content.t = "";
			gravity->content.t = "";
			jumpCount->content.t = "";
		}
		
		centre(target->w, target->h);//Centers on screen
		
		DARK;
		SDL_Surface* old = SDL_CreateRGBSurface(SDL_SWSURFACE, target->w, target->h, 32, 0, 0, 0, 0);//Surface to store target
		SDL_BlitSurface(target, NULL, old, NULL);//Blits target on surface
		
		SDL_Event e;//Event
		
		while (!quitFlag || *quitFlag){//While running
			if (frameBegin) frameBegin();//Frame beginnig
			
			while (SDL_PollEvent(&e)){//While there are events on stack
				if (events) events(e);//Checks events
				
				dialogWindow.checkEvents(e);//Checks dialog events
				
				if (ok->release.triggered){//If pressed ok
					rules *result = new rules;//Result rules
					
					result->setMask = 0;//Resets mask
					
					//Sets members
					if (jumpImpulse->content.t != ""){ result->setMask |= 0b10000000; result->jumpImpulse = atof(jumpImpulse->content.t.c_str()); }
					if (groundSpeed->content.t != ""){ result->setMask |= 0b01000000; result->groundSpeed = atof(groundSpeed->content.t.c_str()); }
					if (groundForce->content.t != ""){ result->setMask |= 0b00100000; result->groundForce = atof(groundForce->content.t.c_str()); }
					if (groundDamping->content.t != ""){ result->setMask |= 0b00010000; result->groundDamping = atof(groundDamping->content.t.c_str()); }
					if (airSpeed->content.t != ""){ result->setMask |= 0b00001000; result->airSpeed = atof(airSpeed->content.t.c_str()); }
					if (airForce->content.t != ""){ result->setMask |= 0b00000100; result->airForce = atof(airForce->content.t.c_str()); }
					if (gravity->content.t != ""){ result->setMask |= 0b00000010; result->gravity.fromString(gravity->content.t.c_str()); }
					if (jumpCount->content.t != ""){ result->setMask |= 0b00000001; result->jumpCount = atoi(jumpCount->content.t.c_str()); }
					
					return result;//Returns result
				}
										
				if (cancel->release.triggered)//If pressed cancel
					return NULL;//Returns null
			}
			
			SDL_BlitSurface(old, NULL, target, NULL);//Prints target
			dialogWindow.print(target);//Prints dialog
			if (update) update();//Updates
			
			if (frameEnd) frameEnd();//Frame ending
		}
	}
} rulesDialog;