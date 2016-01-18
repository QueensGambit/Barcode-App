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
	bool stepByStep, showAllSteps, search, webcam;
	const char* file;
	char webcamVersion; //i(ntern) | e(xter)
	char webcamStyle; //s(ingle frame) | m(any frames (loop))

public:
	SettingObject(const char* execute, bool stepByStep=0, bool showAllSteps=0, bool search=0, bool webcam=0,
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

	void printSettings();
};

#endif /* SETTINGOBJECT_H_ */
