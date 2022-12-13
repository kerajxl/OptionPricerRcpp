#include<iostream>
#include<cmath>
#include"getOneGaussianByBoxMueller.h"
#include"EuropeanOption.h"
#include<numeric>

//definition of constructor
EuropeanOption::EuropeanOption(
	int nInt_,
	double strike_,
	double spot_,
	double vol_,
	double r_,
	double expiry_,
	double barrier_){
		nInt = nInt_;
		strike = strike_;
		spot = spot_;
		vol = vol_;
		r = r_;
		expiry = expiry_;
		barrier = barrier_;
		generatePath();
}

//method definition
void EuropeanOption::generatePath(){
  double thisDrift = (r * expiry - 0.5 * vol * vol * expiry) / double(nInt);
  //double thisDrift = (r * expiry - 0.5 * vol * vol * expiry) ;
	double cumShocks = 0;
	thisPath.clear();
	barFlag.clear();

	for(int i = 0; i < nInt; i++){
    cumShocks += (thisDrift + vol * sqrt(expiry / double(nInt)) * getOneGaussianByBoxMueller());
		//cumShocks += (thisDrift + vol * sqrt(expiry) * getOneGaussianByBoxMueller());
		thisPath.push_back(spot * exp(cumShocks));
		barFlag.push_back((spot * exp(cumShocks) <= barrier ) ? 1 : 0);
	}
}

//method definition
double EuropeanOption::getLastPrice(){
  
  double lastPrice;
  int n = thisPath.size();
  double lPrice = thisPath.back() ;
  
  return lPrice;
}



//method definition
double EuropeanOption::getEuropeanCallPrice(int nReps){

	double rollingSum = 0.0;
	double thisLast = 0.0;

	for(int i = 0; i < nReps; i++){
		generatePath();
		thisLast=getLastPrice();
		
		rollingSum += ((thisLast > strike)&& (accumulate(barFlag.begin(), barFlag.end(), 0) > 0)) ? ( thisLast - strike) : 0;
	}

	return exp(-r*expiry)*rollingSum/double(nReps);

}


//overloaded operator ();
double AsianOption::operator()(char char1, char char2, int nReps){
	if ((char1 == 'A') & (char2 =='C'))      return getEuropeanCallPrice(nReps);
	else return -99;
}
