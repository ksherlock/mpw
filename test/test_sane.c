#include <Math.h>
#include <SANE.h>

#include <stdio.h>

//extern pascal extended NAN(short x);

void dump_decimal(const decimal *d)
{
	fprintf(stdout, "%d : %d : %.*s\n", 
		d->sgn, d->exp, d->sig.length, d->sig.text);
}


void test_inf(void)
{

	decimal d;
	decform df;
	char buffer[80+1];

	printf("\nInfinity\n");

	df.style = 1;
	df.digits = 10;
	num2dec(&df,-inf(), &d);
	dump_decimal(&d);
	//
	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);

	df.style = 0;
	df.digits = 10;
	num2dec(&df, -inf(), &d);
	dump_decimal(&d);
	//
	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);

	df.style = 1;
	df.digits = 0;
	num2dec(&df, inf(), &d);
	dump_decimal(&d);
	//
	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);

	df.style = 0;
	df.digits = 0;
	num2dec(&df, inf(), &d);
	dump_decimal(&d);
	//
	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);

}

void test_nan(void)
{


	// num2dec generates N[16-digit]
	// str2dec generates NAN(001) -- NAN(255)

	decimal d;
	decform df;
	char buffer[80+1];

	printf("\nNaN\n");

	df.style = 1;
	df.digits = 0;
	num2dec(&df,nan(1), &d);
	dump_decimal(&d);
	//  7 : N4001000000000000
	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);

	df.style = 0;
	df.digits = 0;
	num2dec(&df, nan(2), &d);
	dump_decimal(&d);
	//
	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);


	df.style = 1;
	df.digits = 10;
	num2dec(&df,-nan(3), &d);
	dump_decimal(&d);
	//
	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);

	df.style = 0;
	df.digits = 10;
	num2dec(&df, nan(4), &d);
	dump_decimal(&d);
	//
	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);

	df.style = 0;
	df.digits = 10;
	num2dec(&df, nan(255), &d);
	dump_decimal(&d);
	//  7 : N40FF000000000000

	dec2str(&df, &d, buffer);
	fprintf(stdout, "%s\n", buffer);
}

void test_fxc2dec(void)
{
	decimal d;
	decform df;

	df.style = 0;
	df.digits = 100;
	num2dec(&df, 1.125, &d);
	dump_decimal(&d);
	// 0 : -18 : 1125000000000000000

	df.style = 0;
	df.digits = 10;
	num2dec(&df, 1.125, &d);
	dump_decimal(&d);
	// 0 : -9 : 1125000000

	df.style = 0;
	df.digits = 2;
	num2dec(&df, 1.125, &d);
	dump_decimal(&d);
	// 0 : -1 : 11



	df.style = 1;
	df.digits = 100;
	num2dec(&df, 1.125, &d);
	dump_decimal(&d);
	// 0 : -18 : 1125000000000000000

	df.style = 1;
	df.digits = 10;
	num2dec(&df, 1.125, &d);
	dump_decimal(&d);
	// 0 : -10 : 11250000000


	df.style = 1;
	df.digits = 2;
	num2dec(&df, 1.125, &d);
	dump_decimal(&d);
	//0 : -2 : 112

	//

	df.style = 0;
	df.digits = 100;
	num2dec(&df, 1e25, &d);
	dump_decimal(&d);
	// 0 : 7 : 1000000000000000000

	df.style = 0;
	df.digits = 10;
	num2dec(&df, 1e25, &d);
	dump_decimal(&d);
	// 0 : 16 : 1000000000

	df.style = 0;
	df.digits = 2;
	num2dec(&df, 1e25, &d);
	dump_decimal(&d);
	// 0 : 24 : 10



	df.style = 1;
	df.digits = 100;
	num2dec(&df, 1e25, &d);
	dump_decimal(&d);
	// 0 : 7 : 1000000000000000000

	df.style = 1;
	df.digits = 10;
	num2dec(&df, 1e25, &d);
	dump_decimal(&d);
	// 0 : 7 : 1000000000000000000


	df.style = 1;
	df.digits = 2;
	num2dec(&df, 1e25, &d);
	dump_decimal(&d);
	//0 : 7 : 1000000000000000000



	df.style = 1;
	df.digits = 10;
	num2dec(&df, 0, &d);
	dump_decimal(&d);
	//

	df.style = 0;
	df.digits = 10;
	num2dec(&df, 0, &d);
	dump_decimal(&d);
	//


	df.style = 1;
	df.digits = 0;
	num2dec(&df, -0.0, &d);
	dump_decimal(&d);
	//

	df.style = 0;
	df.digits = 0;
	num2dec(&df, -0.0, &d);
	dump_decimal(&d);
	//







	df.style = 0;
	df.digits = 10;
	num2dec(&df, 0.125, &d);
	dump_decimal(&d);
	// 

	df.style = 1;
	df.digits = 10;
	num2dec(&df, 0.125, &d);
	dump_decimal(&d);
	//

	df.style = 0;
	df.digits = 4;
	num2dec(&df, 0.00000125, &d);
	dump_decimal(&d);
	// s/b -9 1250

	df.style = 1;
	df.digits = 4;
	num2dec(&df, 0.00000125, &d);
	dump_decimal(&d);
	// s/b -4 0

}

int main(int argc, char **argv)
{
	//extended x;
	//decimal d;

	//short index;
	//short valid;

	(void)argc;
	(void)argv;

	//index = 0;
	//valid = 0;
	//str2dec("1.125", &index, &d, &valid);
	//x = dec2num(&d);


	test_inf();
	test_nan();

	test_fxc2dec();

	return 0;
}
