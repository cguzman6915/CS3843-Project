// EncryptData.cpp
//
// This file uses the input data and key information to encrypt the input data
//

#include "Main.h"

void encryptData_01(char* data, int datalength)
{

	__asm
	{		 
		// Get the password hash address
		mov		esi, gptrPasswordHash

		// Calculate starting index
			// gPasswordHash[0+round*4] * 256
		movzx	eax, byte ptr[esi]
		shl		eax, 8

			// gPasswordHash[1+round*4]
		movzx	edx, byte ptr[esi + 1 /*+ round * 4*/]
			// index
		add		eax, edx

		// Get key and input data addresses
			// gKey[0]
		mov		esi, gptrKey
		mov		edi, data

		// Get key byte at starting index
			// gKey[index]
		mov		dl, byte ptr[esi + eax]

		// Set loop counter
			// for (x < datalength)
		mov		ecx, datalength

		// Check for empty input
		test	ecx, ecx
		jle		encrypt_done

	encrypt:
			// data[x] = data[x] ^ gKey[index]
		xor		byte ptr[edi], dl

			// x++
		inc		edi
		dec		ecx

		jnz		encrypt
	encrypt_done:
	}

	return;
} // encryptData_01


void encryptData_02(char* data, int datalength)
{
	//char opA[] = "\nCompleted Op A\n";
	//char opB[] = "Completed Op B\n";
	//char opC[] = "Completed Op C\n";
	//char opD[] = "Completed Op D\n";
	//char opE[] = "Completed Op E\n";

	__asm {
		// Get the password hash address
		mov		esi, gptrPasswordHash

		// Calculate starting index
		movzx	eax, byte ptr[esi]
		shl		eax, 8

		movzx	edx, byte ptr[esi + 1 /*+ round * 4*/]
		add		eax, edx

		// Get key and input data addresses
		mov		esi, gptrKey
		mov		edi, data

		// Get key byte at starting index
		mov		dl, byte ptr[esi + eax]

		// Set loop counter
		mov		ecx, datalength

		// Check for empty input
		test	ecx, ecx
		jle		encrypt_done

	encrypt:
		//lea		esi, byte ptr [edi]
		//push	esi
		//call	printf
		//add		esp, 4
		//push	500
		//call	Sleep

		/* Milestone 1 */
			xor		byte ptr[edi], dl

		/* C-INVERT BITS 0,1,6,7 */
			xor		byte ptr[edi], 0C3h

			//lea		esi, opC
			//push	esi
			//call	printf
			//add		esp, 4

		/* D-REVERSE BIT ORDER */
			// Load data[x] into al
			mov		al, byte ptr[edi]

			// Swap nibbles
			ror		al, 4

			// Swap pairs of bits
			mov		bl, al
			and		al, 0CCh
			shr		al, 2
			and		bl, 033h
			shl		bl, 2
			or		al, bl

			//		Swap individual bits
			mov		bl, al
			and		al, 0AAh
			shr		al, 1
			and		bl, 055h
			shl		bl, 1
			or		al, bl

			// Store reversed byte
			mov		byte ptr[edi], al

			//lea		esi, opD
			//push	esi
			//call	printf
			//add		esp, 4

		/* A-ROTATE NIBBLES LEFT */
			// Load data[x] into al and ah (dl holds the key, don't touch it)
			mov		al, byte ptr[edi]
			mov		ah, al

			// Rotate upper nibble left
			and		al, 70h
			shl		al, 1
			and		ah, 80h
			shr		ah, 3
			or		al, ah

			// Load data[x] into bl and ah
			mov		bl, byte ptr[edi]
			mov		ah, bl

			// Rotate lower nibble left
			and		bl, 07h
			shl		bl, 1
			and		ah, 08h
			shr		ah, 3
			or		bl, ah

			// Combine both nibbles
			or		al, bl

			// Store result
			mov		byte ptr[edi], al

			//lea		esi, opA
			//push	esi
			//call	printf
			//add		esp, 4

		/* E-ROTATE GH BITS RIGHT */
			// X == 0 (ecx counts down from datalength, so the first byte is ecx == datalength)
			cmp		ecx, datalength
			je		OP_E_DONE

			// load byte and previous byte
			mov		al, byte ptr [edi-1]
			mov		bl, byte ptr [edi]

			// use AND to mask al (previous)
			and		al, 03h

			// jump to correct section using CMPs
			cmp		al, 0
			je		OP_E_DONE
			cmp		al, 1
			je		JMP_01
			cmp		al, 2
			je		JMP_10
			cmp		al, 3
			je		JMP_11
		
			// rotate 1, 2, or 3 bits
		JMP_01:
			ror		bl, 1
			jmp		OP_E_STORE
		JMP_10:
			ror		bl, 2
			jmp		OP_E_STORE
		JMP_11:
			ror		bl, 3
			jmp		OP_E_STORE

			// store new byte into memory
		OP_E_STORE:
			mov		byte ptr[edi], bl
		OP_E_DONE:

			//lea		esi, opE
			//push	esi
			//call	printf
			//add		esp, 4

		/* B-CODE TABLE SWAP */
			// load address of gEncodeTable
			lea		ebx, gEncodeTable

			// load current byte zero extended into eax to use it as an offset
			movzx	eax, byte ptr [edi]
			
			// load appropriate gEncodeTable byte into al (dl holds the key, don't touch it)
			mov		al, byte ptr [ebx + eax]

			// store al into memory
			mov		byte ptr [edi], al

			//lea		esi, opB
			//push	esi
			//call	printf
			//add		esp, 4

		inc		edi
		dec		ecx
		jnz		encrypt
	encrypt_done:
	}

	return;
} // encryptData_02


void encryptData_03(char* data, int datalength)
{

	__asm
	{
		nop		// REPLACE THIS WITH YOUR ENCRYPT CODE FOR MILESTONE #3
	}

	return;
} // encryptData_03


// YOU WILL DELETE ALL THIS CODE BELOW WHEN YOU DO YOUR PROJECT - THIS IS GIVING YOU EXAMPLES OF HOW TO 
// ACCESS gPasswordHash AND gKey

//////////////////////////////////////////////////////////////////////////////////////////////////
// EXAMPLE code to to show how to access global variables
//int encryptData(char *data, int dataLength)
//{
//	int resulti = 0;
//
//	gdebug1 = 0;				// a couple of global variables that could be used for debugging
//	gdebug2 = 0;				// also can have a breakpoint in C code
//
//	// You can not declare any local variables in C, but should use resulti to indicate any errors
//	// Set up the stack frame and assign variables in assembly if you need to do so
//	// access the parameters BEFORE setting up your own stack frame
//	// Also, you cannot use a lot of global variables - work with registers
//
//	__asm {
//		// you will need to reference some of these global variables
//		// (gptrPasswordHash or gPasswordHash), (gptrKey or gkey), gNumRounds
//
//		// simple example that xors 2nd byte of data with 14th byte in the key file
//		lea esi,gkey				// put the ADDRESS of gkey into esi
//		mov esi,gptrKey;			// put the ADDRESS of gkey into esi (since *gptrKey = gkey)
//
//		lea	esi,gPasswordHash		// put ADDRESS of gPasswordHash into esi
//		mov esi,gptrPasswordHash	// put ADDRESS of gPasswordHash into esi (since unsigned char *gptrPasswordHash = gPasswordHash)
//
//		mov al,byte ptr [esi]				// get first byte of password hash
//		mov al,byte ptr [esi+4]				// get 5th byte of password hash
//		mov ebx,2
//		mov al,byte ptr [esi+ebx]			// get 3rd byte of password hash
//		mov al,byte ptr [esi+ebx*2]			// get 5th byte of password hash
//
//		mov ax,word ptr [esi+ebx*2]			// gets 5th and 6th bytes of password hash ( gPasswordHash[4] and gPasswordHash[5] ) into ax
//		mov eax,dword ptr [esi+ebx*2]		// gets 4 bytes, as in:  unsigned int X = *( (unsigned int*) &gPasswordHash[4] );
//
//		mov al,byte ptr [gkey+ebx]			// get's 3rd byte of gkey[] data
//
//		mov al,byte ptr [gptrKey+ebx]		// THIS IS INCORRECT - will add the address of the gptrKey global variable (NOT the value that gptrKey holds)
//
//		mov al,byte ptr [esi+0xd];			// access 14th byte in gkey[]: 0, 1, 2 ... d is the 14th byte
//		mov edi,data				// Put ADDRESS of first data element into edi
//		xor byte ptr [edi+1],al		// Exclusive-or the 2nd byte of data with the 14th element of the keyfile
//									// NOTE: Keyfile[14] = 0x21, that value changes the case of a letter and flips the LSB
//									// Capital "B" = 0x42 becomes lowercase "c" since 0x42 xor 0x21 = 0x63
//	}
//
//	return resulti;
//} // encryptData

