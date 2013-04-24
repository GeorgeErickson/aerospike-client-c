SECTION I: INSTALLING SWIG
	Installing swig:
	Install swig version 2.0.0 or above from the link http://www.swig.org/download.html

SECTION II: LANGUAGE SPECIFIC
	Note: 1. Before creating the language modules, build the C client. To do that, run 'make' from the C directory.
	2. For creating the language module, in 'swig' directory, run 'make LANG=<lang_name>' where
	   <lang_name> is the language you are creating the module for.  
	3. For cleanup, run 'make clean LANG=<lang_name>'. In make clean, if you do not provide a variable, 
	   all existing language directories will be cleaned up.
	4. Do not add files to the <lang_name> directory. (For running examples etc)
 	5. For all languages. the type of each bin of bins array while calling 'citrusleaf_get()' or 
	   'citrusleaf_get_digest()' should be initialized to 'CL_NULL'. This can be done by calling
	   citrusleaf_object_init_null(cl_object*).
	
	PERL
	Creating perl module
		1. Install PERL and make sure that C libraries for perl also get installed. 
		   Package name is: 'libgnome2-perl'.
		     By default the current perl path is added to "DIR_INCLUDE_PERL" in the Makefile.
		   * If using any other version, change the include path of perl CORE lib.
		   * For ubuntu, run 'sudo apt-get install libgnome2-perl'
		2. In 'swig' directory, run 'make LANG=perl'
		3. perl/perl_citrusleaf.pl is a perl file which has all the helper functions 
		   that an application might need.
		
	Testing perl client 
		1. From examples directory, run 'perl test.pl' /*Citrusleaf library is imported as 
		   'citrusleaf' in this file*/
		2. For writing your own applications, 'use citrusleaf' in your perl file.
		   All functions, unless otherwise specified, are called just like in C. 
		   See more details on the C documentation here - http://citrusleaf.net/apis/capi.php .
   		   For citrusleaf_get_all, after the function is called, the bins are returned in the bins array.
		   To get the value of these bins, use the perl helper function defined in perl_citrusleaf.pl,
		   get_bins(bins,<noofbins>)
		3. There is add on perl file perl/perl_citrusleaf.py which has perl helper functions defined.
		
	Other add ons: (Ref: test.pl)
		1. Defining structures: Call 'new citrusleaf::<structurename>()'. 
		   For examples, see the definition of o_key.
		2. Passing integer pointers: 'citrusleaf::new_intp()' /*Creating an integer pointer to be passed*/
   		   citrusleaf::intp_value(gen) /*Deferencing the integer pointer gen */
   		   These pointeres are declared using pointer class in citrusleaf.i file like %pointer_functions(int,intp).
	     	   Similarly other pointers are declared in this file and are used accordingly.
	       	3. Some tips on how to use pointers in perl:
			   $ptr = citrusleaf::new_intp();
			   $ptr1 = citrusleaf::new_intp();
			   Check if NULL
				   if (defined($ptr)) {
					   print "Not a NULL pointer.";
				   } else {
 					   print "Is a NULL pointer.";
				   }
				Check if 2 pointers point to same C object
				   if ($$ptr == $$ptr1) {
					   print "ptr and ptr1 point to the same object in C.";
				   } else {
					   print "ptr and ptr1 point to different objects.";
				   }
		4. Defining arrays: The arrays are declared in citrusleaf.i as %array_class(<structname>,<arrayname>). 
		   They are defined in test.pl file as <array_name> = new citrusleaf::<arrayname>(<arraysize>).
		   For example, see the use of cl_bin_arr.
		   Similarly other arrays are declared in this file and are used accordingly.
		   Assign values to array members: For each element of array, create a new element
		   and use 'setitem' function of array to set ith 
		   element of array. After setting the ith element, delete the newly created element by using DESTROY() function.
		   'setitem' takes two arguments: $array_name->setitem($i, $element). 
		   This fuction will copy element into the ith index of array_name.
	           To have a look into example see code chunk in file test.pl:78-87.
		   'getitem' takes exactly one argument: $array_name->getitem($i) which returns the ith element of array_name.
		5. Freeing bins - citrusleaf::citrusleaf_free_bins(cl_bin *, int , cl_bin **) takes three arguments, 
		   the array of cl_bins, the size of the array and a cl_bin **. If we want to free just the array,
		   we pass a 'undef'(without quotes) for cl_bin ** and vice versa. 
		6. Using blobs - For getting the blobs back, use citrusleaf::cdata(blob,size),
		   where 'blob' is the blob object that is returned and 'size' is the object size. For example, 
		   binary_data = citrusleaf::cdata($bins_get->getitem($i)->{object}->{u}->{blob}, $bin->{object}->{sz}) 
		   where 'bins_get' is the array of bins returned after the get call.
	    	7. Accessing constants: constants can be defined by define statement or it could be an enumeration
	       	   ex: #define FOO 35
	       	   Access FOO in perl code by writing citrusleaf::FOO
		8. More than 32bit integers:
		   64 bit integers gets converted into string while interfacing with C via perl.
		   PERL is able to perform arithmetic operation on string only after converting them into real numbers.
		   String value gets converted into double if value exceeds signed-64-bit value, which will lose precision.
		   To avoid this use 'Math::BigInt'. Here is an example:
				// 'example' is a shared object created by using swig.
				// 'var_uint64' is a global variable.
				use example;
				$var_uint64 = $example::var_uint64;
				Expected:
					print $var_uint64;		// prints 9223372036854775807
					print $var_uint64 + 1;	// prints 9223372036854775808
				Reality:
					print $var_uint64;		// prints 9223372036854775807
					print $var_uint64 + 1;	// prints 9.22337203685478e+18
				Recommended coding style:
					use Math::BigInt;
					$var_uint64 = Math::BigInt->new($var_uint64);
					print $var_uint64;		// prints 9223372036854775807
					print $var_uint64 + 1;	// prints 9223372036854775808
	
