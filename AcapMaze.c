#pragma config(Sensor, S1,     eyes,           sensorEV3_Ultrasonic)
#pragma config(Sensor, S2,     garis,          sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     gyro,           sensorEV3_Gyro)
#pragma config(Motor,  motorA,          right,         tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorB,          head,          tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorD,          left,          tmotorEV3_Large, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int choices = 0;
char optimizedDecisions[20];
int Tp = 40;
int options = 0;
int turnDeg = 84;
bool dahNampak = false;
char decisions[20];
int done = 0;
//pusing -1 for left 1 for right

void depan(float masa){
	resetTimer(T1);
	while(getTimer(T1, seconds) < masa){
		setMotorSpeed(left, Tp);
		setMotorSpeed(right, Tp);
	}
	stopAllMotors();
	wait1Msec(250);
}

//pusing -1 for left 1 for right
void pusing(int where){

	resetGyro(gyro);
	wait1Msec(500);
	if(where != 0){
		while(abs(getGyroDegrees(gyro)) < turnDeg){
				setMotorSpeed(left, -Tp/5 * where);
				setMotorSpeed(right, Tp/5 * where);
		}

		setMotorSpeed(left, -5 * where);
		setMotorSpeed(right, 5 * where);
		wait1Msec(250);
	}
	stopAllMotors();

}

void depanSampai(float gapLeft, float gapFront){
	float front, kiri, kanan;
	front = 0;
	kiri = 0;
	kanan = 0;

	while(getColorName(garis) != colorBlack){
		//look forward
		front = getUSDistance(eyes);
		wait1Msec(250);
		setMotorTarget(head, -90, 20);
		waitUntilMotorStop(head);
		wait1Msec(250);

		//look left
		kiri = getUSDistance(eyes);
		wait1Msec(250);
		setMotorTarget(head, 91, 20);
		waitUntilMotorStop(head);
		wait1Msec(250);

		//look right
		kanan = getUSDistance(eyes);
		wait1Msec(250);
		setMotorTarget(head, 0, 20);
		waitUntilMotorStop(head);
		wait1Msec(250);

		if (front < (gapFront * 1.05) || kiri > (gapLeft * 1.5) || kanan > (gapLeft * 1.5)){
			stopAllMotors();
			wait1Msec(250);
			return;
		}
		else{
			depan(0.5);
		}
	}
	playSound(soundBeepBeep);
	dahNampak = true;
}

void tengok(float gapLeft){
	float scans[3];
	options = 0;

	setMotorTarget(head, -90, 20);
	waitUntilMotorStop(head);
	scans[0] = getUSDistance(eyes);
	wait1Msec(250);

	setMotorTarget(head, 0, 20);
	waitUntilMotorStop(head);
	scans[1] = getUSDistance(eyes);
	wait1Msec(250);

	setMotorTarget(head, 91, 20);
	waitUntilMotorStop(head);
	scans[2] = getUSDistance(eyes);
	wait1Msec(250);

	setMotorTarget(head, 0, 20);
	waitUntilMotorStop(head);
	wait1Msec(250);

	float gap = 2 * gapLeft;
	if (scans[0] > gap)
		options = options + 1;
	if (scans[1] > 60)
		options = options + 2;
	if (scans[2] > gap)
		options = options + 4;
}

void compile(char option){
	decisions[choices] = option;
	choices++;
	for (int i = 0; i < choices; i++){
		playSound(soundBlip);
		wait1Msec(250);
	}
}

void decide(){
	if (options == 1 || options == 3 || options == 5 || options == 7){//left
		depan(2.5);
		pusing(-1);
		depan(2.5);
		compile('L');
	}
	else if(options == 2 || options == 6){//forward
		depan(5);
		compile('F');
	}
	else if(options == 4){//right
		depan(2.5);
		pusing(1);
		depan(2.5);
		compile('R');
	}
	else{//back
		pusing(-1);
		pusing(-1);
		depan(2.5);
		compile('B');
	}
}

void simplify(){
	int before, current, after;
	//int done = 0;
	for (int i = 0; i < choices; i++){
		if(i == 0){
			optimizedDecisions[done] = decisions[i];
			done++;
		}
		else{
			before = decisions[i-1];
			current = decisions[i];
			after = decisions[i+1];
			if(current == 'B'){
				if(before == 'F' && after == 'R'){
					done--;
					optimizedDecisions[done] = 'L';
					done++;
					playSound(soundBlip);
					wait1Msec(200);
					i++;
				}
				else if(before == 'F' && after == 'L'){
					done--;
					optimizedDecisions[done] = 'R';
					done++;
					playSound(soundBlip);
					wait1Msec(200);
					i++;
				}
				else if(before == 'L' && after == 'L'){
					done--;
					optimizedDecisions[done] = 'R';
					done++;
					playSound(soundBlip);
					wait1Msec(200);
					i++;
				}
				else if(before == 'L' && after == 'F'){
					done--;
					optimizedDecisions[done] = 'R';
					done++;
					playSound(soundBlip);
					wait1Msec(200);
					i++;
				}
			}
			else if (after == '0'){
				optimizedDecisions[done] = current;
			}
			else{
				optimizedDecisions[done] = current;
				done++;
			}
		}
		for (int y = 0; y < done; y++){
			playSound(soundDownwardTones);
			wait1Msec(250);
		}
	}
}

void reverse(){
	float gapLeft = 22.0;
	float gapFront = 20.0;

	int j = 0;
	char reverseDecisions[20];

	for (int d = 0; d < 20; d++)
		reverseDecisions[d] = '0';

	for (int i = done; i >= 0; i--){
		if(optimizedDecisions[i] == 'L'){
			reverseDecisions[j] = 'R';
			playSound(soundShortBlip);
			j++;
			wait1Msec(2000);
		}
		else if (optimizedDecisions[i] == 'R'){
			reverseDecisions[j] = 'L';
			playSound(soundShortBlip);
			j++;
			wait1Msec(2000);
		}
		else if (optimizedDecisions[i] == 'F'){
			reverseDecisions[j] = 'F';
			playSound(soundShortBlip);
			j++;
			wait1Msec(2000);
		}
	}

	for (int l = 0; l < j; l++){
		playSound(soundBlip);
		wait1Msec(2000);
	}
	wait1Msec(2000);

	for (int k = 0; k < j; k++){
		depanSampai(gapLeft, gapFront);
		if (reverseDecisions[k] == 'R'){
			depan(2.5);
			pusing(1);
			depan(2.5);
		}
		else if (reverseDecisions[k] == 'L'){
			depan(2.5);
			pusing(-1);
			depan(2.5);
		}
		else if (reverseDecisions[k] == 'F'){
			depan(5);
		}
	}
	stopAllMotors();
}

task main(){

	float gapLeft = 22.0;
	float gapFront = 20.0;

	for (int i = 0; i < 20; i++){
		decisions[i] = '0';
		optimizedDecisions[i] = '0';
	}

	while(true){
		depanSampai(gapLeft, gapFront);
		if(dahNampak)
			break;
		tengok(gapLeft);
		decide();
	}

	wait1Msec(2000);

	pusing(-1);
	pusing(-1);

	wait1Msec(2000);

	simplify();
	playSound(soundBeepBeep);
	reverse();
	depan(2);
}
