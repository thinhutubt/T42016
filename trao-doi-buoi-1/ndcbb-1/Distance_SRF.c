float Distance_SRF(){
	unsigned long times;
	float distance;

	digitalWrite(trigger,HIGH);
	delayMicroseconds(1000); //Giu mot khoang thoi gian de phat xung di
	digitalWrite(trigger,LOW);
	times=pulseIn(echo,HIGH);//Lay thoi gian chan Echo tu HIGH --> LOW
	distance=0.0344*(times/2); // Van toc song am: v = 344 m/s
	return distance;
}

