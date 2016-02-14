/*
 * SettingObject.h
 *
 *  Created on: 18.01.2016
 *      Author: Anonym
 */

#ifndef SETTINGOBJECT_H_
#define SETTINGOBJECT_H_

#include <iostream>
using namespace std;

class SettingObject {
private:
	const char* execute;
	//attributes mean the following:
	//  go on manually by pressing any key, show all steps of the algorimth, perform an internet search, webcam is selected, text2speech is selected
	bool stepByStep, showAllSteps, search, webcam, speach;
	const char* file;
	char webcamVersion; //i(ntern) | e(xter)
	char webcamStyle; //s(ingle frame) | m(any frames (loop))
	bool basic = 0; //get_barcode() shows nothing, only returns 1 if a barcode was successfully read
				//and 0 otherwise, -1 for an exception/error

public:
	SettingObject(const char* execute, bool stepByStep=0, bool showAllSteps=0,
			bool search=0, bool webcam=0, bool speach=0,
			const char* file="media/internet/Chips_rotated.jpg",
			char webcamVersion='0', char webcamStyle='0');
	virtual ~SettingObject();
	const char* getFile() const;
	bool isSearch() const;
	bool isShowAllSteps() const;
	bool isStepByStep() const;
	bool isWebcam() const;
	char getWebcamStyle() const;
	char getWebcamVersion() const;
	const char* getExecute() const;
	void setValues(bool, bool, bool, bool, bool,
			const char*,
			char, char);
	void printSettings();
	bool isSpeach() const;
	bool isBasic() const;
	void setBasic(bool basic);
};

#endif /* SETTINGOBJECT_H_ */
