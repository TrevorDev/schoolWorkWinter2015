
int fib(int n) {
   if (n<=1)
      return n;
   else
      return ( fib(n-1) + fib(n-2) );
}

long long int fib1(int n) {
	int fnow = 0, fnext = 1, tempf;
	while(--n>0){
		tempf = fnow + fnext;
		fnow = fnext;
		fnext = tempf;
		}
		return fnext;
}



long long unsigned fib2(unsigned n) {
    return floor( (pow(PHI, n) - pow(1 - PHI, n))/sqrt(5) );
}
