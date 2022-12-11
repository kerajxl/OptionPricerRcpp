#include<iostream>
#include<cmath>
#include"getOneGaussianByBoxMueller.h"
#include"AsianOption.h"
#include<numeric>

//definition of constructor
AsianOption::AsianOption(
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
void AsianOption::generatePath(){
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
double AsianOption::getArithmeticMean(){

	double runningSum = 0.0;

	for(int i = 0; i < nInt; i++){
		runningSum += thisPath[i];
	}

	return runningSum/double(nInt);
}


double AsianOption::getLastPrice(){
  
  double lastPrice;
  int n = thisPath.size();
  double lPrice = thisPath.back() ;
  
  return lPrice;
}



//method definition
void AsianOption::printPath(){

	for(int i = 0;  i < nInt; i++){

		std::cout << thisPath[i] << "\n";

	}

}

//method definition
double AsianOption::getArithmeticAsianCallPrice(int nReps){

	double rollingSum = 0.0;
	double thisMean = 0.0;

	for(int i = 0; i < nReps; i++){
		generatePath();
		thisMean=getLastPrice();
		
		rollingSum += ((thisMean > strike)&& (accumulate(barFlag.begin(), barFlag.end(), 0) > 0)) ? ( thisMean - strike) : 0;
	}

	return exp(-r*expiry)*rollingSum/double(nReps);

}


//overloaded operator ();
double AsianOption::operator()(char char1, char char2, int nReps){
	if ((char1 == 'A') & (char2 =='C'))      return getArithmeticAsianCallPrice(nReps);
	else return -99;
}
