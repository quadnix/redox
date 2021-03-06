#include <iostream>

#include "headers/all.h"

using namespace std;
using namespace redox;

void writeMem(redox::mem::Memory &bm);

int main(int argc, char** argv) {
	config::process_main(argc, argv);
	tools::init_rand();
	cout << "MEMORY TEST\n";
	redox::mem::Memory bmem;
	writeMem(bmem);
	cout << "Wrote memory slot 0\n";
	cout << "Begin read...\n";
	cout << "Name: " << static_cast<redox::mem::types::Int*>(bmem.get_ptr(0))->get_name() << "\n";
	cout << "Value: " << static_cast<redox::mem::types::Int*>(bmem.get_ptr(0))->read() << "\n";
	cout << "Reading name with VarBase cast (!):\n";
	cout << "Name: " << static_cast<redox::mem::types::VarBase*>(bmem.get_ptr(0))->get_name() << "\n";
	cout << "Type: " << bmem.get_type(bmem.search("int1")) << "\n";
	cout << "I/O with mem::Memory wrappers\n";
	cout << "Writing \"C++ FTW\" to a string using a add_item cast with name testStr\n";
	bmem.add_item(new string("C++ FTW"), redox::mem::types::STRING, "testStr");
	cout << "Reading string back...\n";
	cout << "Value: " << static_cast<redox::mem::types::String*>(bmem.get_ptr(bmem.search("testStr")))->read() << '\n';
	cout << "Name: " << bmem.get_name(bmem.search("testStr")) << '\n';
	cout << "\nLIBRARY TEST\n";
	cout << "Creating lib object...\n";
	redox::libs::Lib lib;
	cout << "Initializing with \"exl\" as target...\n";
	cout << "Loading library \"exl\" from [" << tools::get_lib_path() + "exl" << "]\n";
	lib.init(tools::get_lib_path() + "exl", "exl");
	cout << "Checking equeue... ";
	if (error::equeue.are_errors()) {
		cout << "Errors detected! Aborting...\nTrace: \n" << error::equeue.generate_report() << "\n";
		error::equeue.clear();
	} else {
		cout << "empty.\n";
		cout << "Method declarations in \"exl\": " << lib.methodl << "\n";
		cout << "getting library name: " << lib.runfcn("__name", "") << "\n";
		if (error::equeue.responder("getting library name with __name()")) return 1;
	}
	cout << "\nSTL TEST\n";
	libs::Lib term;
	cout << "Loading stl::term...\n";
	term.init(tools::get_lib_path() + "term", "term");
	if (error::equeue.responder("loading \"term\" library")) return 1;
	cout << "Method list: " << term.methodl << "\n";
	cout << "Printing term.__name with term.println...\n";
	term.runfcn("println", term.runfcn("__name",""));
	if (error::equeue.responder("accessing functions in \"term\" stl library")) return 1;
	cout << "Getting input and echoing...\n";
	term.runfcn("print", "name: ");
	term.runfcn("println", term.runfcn("get_line",""));
	if (error::equeue.responder("accessing functions in \"term\" stl library")) return 1;
	cout << "echoing complete.\n";
	
	cout << "\nPROCESSING TESTS\n";
	cout << "Loading file...\nfile to load: ";
	core::File file; file.load_from_file(term.runfcn("get_line", ""));
	if (error::equeue.responder("opening file for reading")) return 1;
	for (int c = 0; c < file.raw.length(); c++) {
		cout << file.raw.get_line(c) << '\n';
	}
	cout << "preprocessing...\n";
	prep::prep_file(file);
	runner::Parser ps;
	for (int c = 0; c < file.raw.length(); c++) {
		ps.clear();
		ps.parseln(file.raw.get_line(c));
		cout << file.raw.get_line(c).length() << "\t" << tools::optype_to_string_fp(ps.optype) << "\t| " << file.raw.get_line(c) << '\n';
	}
	cout << "\nTOOLKIT TEST\nGenerating a few random UIDs: \n";
	for (int c = 0; c < 15; c++) {
		cout <<  "\t" << tools::gen_UID() << "\n";
	}
	cout << "Loading UID library...\n";
	libs::Lib uid;
	uid.init(tools::get_lib_path() + "uid", "uid");
	if (error::equeue.responder("loading UID library")) return 1;
	cout << "Loaded UID library successfully.\nMethods exposed by UID: " << uid.methodl;
	cout << "\nGetting a UID with uid.generate(): [" << uid.runfcn("generate", "") << "]\n";
	if (error::equeue.responder("getting a UID")) return 1;
	
	return 0;
}

void writeMem(redox::mem::Memory &bm) {
	static_cast<redox::mem::types::Int*>(bm.get_ptr(bm.add_slot(redox::mem::types::INT)))->init(10, "int1");
}
