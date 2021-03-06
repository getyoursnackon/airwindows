/* ========================================
 *  Dark - Dark.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __Dark_H
#include "Dark.h"
#endif

void Dark::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];	
	int processing = (VstInt32)( A * 1.999 );
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int depth = (int)(17.0*overallscale);
	if (depth < 3) depth = 3;
	if (depth > 98) depth = 98;
	bool highres = false;
	if (processing == 1) highres = true;
    
    while (--sampleFrames >= 0)
    {
		long double inputSampleL = *in1;
		long double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-37) inputSampleL = fpd * 1.18e-37;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		if (fabs(inputSampleR)<1.18e-37) inputSampleR = fpd * 1.18e-37;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		
		if (highres) {
			inputSampleL *= 8388608.0;
			inputSampleR *= 8388608.0;
		} else {
			inputSampleL *= 32768.0;
			inputSampleR *= 32768.0;
		}
		//0-1 is now one bit, now we dither
		//We are doing it first Left, then Right, because the loops may run faster if
		//they aren't too jammed full of variables. This means re-running code.
		
		//begin left
		int quantA = floor(inputSampleL);
		int quantB = floor(inputSampleL+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		float expectedSlew = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlew += (lastSampleL[x+1] - lastSampleL[x]);
		}
		expectedSlew /= depth; //we have an average of all recent slews
		//we are doing that to voice the thing down into the upper mids a bit
		//it mustn't just soften the brightest treble, it must smooth high mids too
		
		float testA = fabs((lastSampleL[0] - quantA) - expectedSlew);
		float testB = fabs((lastSampleL[0] - quantB) - expectedSlew);
		
		if (testA < testB) inputSampleL = quantA;
		else inputSampleL = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			lastSampleL[x+1] = lastSampleL[x];
		}
		lastSampleL[0] = inputSampleL;
		//end left
		
		//begin right
		quantA = floor(inputSampleR);
		quantB = floor(inputSampleR+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		expectedSlew = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlew += (lastSampleR[x+1] - lastSampleR[x]);
		}
		expectedSlew /= depth; //we have an average of all recent slews
		//we are doing that to voice the thing down into the upper mids a bit
		//it mustn't just soften the brightest treble, it must smooth high mids too
		
		testA = fabs((lastSampleR[0] - quantA) - expectedSlew);
		testB = fabs((lastSampleR[0] - quantB) - expectedSlew);
		
		if (testA < testB) inputSampleR = quantA;
		else inputSampleR = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			lastSampleR[x+1] = lastSampleR[x];
		}
		lastSampleR[0] = inputSampleR;
		//end right
		
		if (highres) {
			inputSampleL /= 8388608.0;
			inputSampleR /= 8388608.0;
		} else {
			inputSampleL /= 32768.0;
			inputSampleR /= 32768.0;
		}
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Dark::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	int processing = (VstInt32)( A * 1.999 );
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int depth = (int)(17.0*overallscale);
	if (depth < 3) depth = 3;
	if (depth > 98) depth = 98;
	bool highres = false;
	if (processing == 1) highres = true;
	
    while (--sampleFrames >= 0)
    {
		long double inputSampleL = *in1;
		long double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-43) inputSampleL = fpd * 1.18e-43;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		if (fabs(inputSampleR)<1.18e-43) inputSampleR = fpd * 1.18e-43;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		
		if (highres) {
			inputSampleL *= 8388608.0;
			inputSampleR *= 8388608.0;
		} else {
			inputSampleL *= 32768.0;
			inputSampleR *= 32768.0;
		}
		//0-1 is now one bit, now we dither
		//We are doing it first Left, then Right, because the loops may run faster if
		//they aren't too jammed full of variables. This means re-running code.
		
		//begin left
		int quantA = floor(inputSampleL);
		int quantB = floor(inputSampleL+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		float expectedSlew = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlew += (lastSampleL[x+1] - lastSampleL[x]);
		}
		expectedSlew /= depth; //we have an average of all recent slews
		//we are doing that to voice the thing down into the upper mids a bit
		//it mustn't just soften the brightest treble, it must smooth high mids too
		
		float testA = fabs((lastSampleL[0] - quantA) - expectedSlew);
		float testB = fabs((lastSampleL[0] - quantB) - expectedSlew);
		
		if (testA < testB) inputSampleL = quantA;
		else inputSampleL = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			lastSampleL[x+1] = lastSampleL[x];
		}
		lastSampleL[0] = inputSampleL;
		//end left
		
		//begin right
		quantA = floor(inputSampleR);
		quantB = floor(inputSampleR+1.0);
		//to do this style of dither, we quantize in either direction and then
		//do a reconstruction of what the result will be for each choice.
		//We then evaluate which one we like, and keep a history of what we previously had
		
		expectedSlew = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlew += (lastSampleR[x+1] - lastSampleR[x]);
		}
		expectedSlew /= depth; //we have an average of all recent slews
		//we are doing that to voice the thing down into the upper mids a bit
		//it mustn't just soften the brightest treble, it must smooth high mids too
		
		testA = fabs((lastSampleR[0] - quantA) - expectedSlew);
		testB = fabs((lastSampleR[0] - quantB) - expectedSlew);
		
		if (testA < testB) inputSampleR = quantA;
		else inputSampleR = quantB;
		//select whichever one departs LEAST from the vector of averaged
		//reconstructed previous final samples. This will force a kind of dithering
		//as it'll make the output end up as smooth as possible
		
		for(int x = depth; x >=0; x--) {
			lastSampleR[x+1] = lastSampleR[x];
		}
		lastSampleR[0] = inputSampleR;
		//end right
		
		if (highres) {
			inputSampleL /= 8388608.0;
			inputSampleR /= 8388608.0;
		} else {
			inputSampleL /= 32768.0;
			inputSampleR /= 32768.0;
		}
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
