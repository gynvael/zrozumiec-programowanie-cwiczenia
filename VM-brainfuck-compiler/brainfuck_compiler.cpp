#include "brainfuck_compiler.h"

#include <fstream>
#include <stack>

template <>
inline BrainfuckCompiler::LittleEndianOFStream & BrainfuckCompiler::LittleEndianOFStream::operator<<(const uint8_t data)
{
	static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");
	put(data);
	return *this;
}

template <>
inline BrainfuckCompiler::LittleEndianOFStream & BrainfuckCompiler::LittleEndianOFStream::operator<<(const char* data)
{
	static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");
	do
	{
		put(*data);
	} while (*(data++));
	return *this;
}

bool BrainfuckCompiler::compile(std::string out_name)
{
	std::ifstream source(m_file_name.c_str());
	LittleEndianOFStream target(out_name.c_str());
	if (!target.is_open())
	{
		m_error_list.push_back("Cannot create file " + out_name + "!");
		return false;
	}
	constexpr int memory_address_offset = 2;
	target << uint8_t(0x01) << uint8_t(0x00) << uint32_t(0xFFFFFFFF); // set r0 as current memory address
	target << uint8_t(0x01) << uint8_t(0x01) << uint32_t(0x00000001); // set r1 as 1
	target << uint8_t(0x01) << uint8_t(0x02) << uint32_t(0x00000000); // set r2 as 0
	target << uint8_t(0x01) << uint8_t(0x03) << uint32_t(0x00000000); // set r3 as temporary register
	target << uint8_t(0x00) << uint8_t(0x04) << uint8_t(0x00); // set r4 to address of beginning of memory
	std::stack<uint32_t> loop_stack; // stack containing addresses of beginnings of loops body
	std::stack<uint32_t> decr_pointer_stack; // stack containing addresses of instruction one after jump to negative pointer error block
	std::stack<int> pointer_counter;
	pointer_counter.push(0);
	bool negative_pointer_threat = false;
	bool load_data = true;
	char c;
	while ((c = source.get()) != EOF)
	{
		switch (c)
		{
		case '>':
			pointer_counter.top()++;
			if(load_data)
				target << uint8_t(0x05) << uint8_t(0x00) << uint8_t(0x03); // VSTB r0, r3; store byte from r3 in address from r0
			target << uint8_t(0x10) << uint8_t(0x00) << uint8_t(0x01); // VADD r0, r1; r0++
			if (source.peek() != '>' && source.peek() != '<')
			{
				target << uint8_t(0x04) << uint8_t(0x03) << uint8_t(0x00); //VLDB r3, r0; load byte from address from r0 to r3
				load_data = true;
			}
			else
			{
				load_data = false;
			}
			break;
		case '<':
			pointer_counter.top()--;
			if (load_data)
				target << uint8_t(0x05) << uint8_t(0x00) << uint8_t(0x03); // VSTB r0, r3; store byte from r3 in address from r0
			target << uint8_t(0x11) << uint8_t(0x00) << uint8_t(0x01); // VSUB r0, r1; r0--
			if (pointer_counter.top() < 0)
			{
				negative_pointer_threat = true;
				target << uint8_t(0x20) << uint8_t(0x00) << uint8_t(0x04); //VCMP  r0, r4; compare r4 and r0
				target << uint8_t(0x23) << uint16_t(0xFFFF); //VJB imm16; if r0 < r4 jump to block handles negative pointer error
				decr_pointer_stack.push(target.tellp());
			}
			if (source.peek() != '>' && source.peek() != '<')
			{
				target << uint8_t(0x04) << uint8_t(0x03) << uint8_t(0x00); //VLDB r3, r0; load byte from address from r0 to r3
				load_data = true;
			}
			else
			{
				load_data = false;
			}
			break;
		case '+':
			target << uint8_t(0x10) << uint8_t(0x03) << uint8_t(0x01); // VADD r3, r1; r3++
			break;
		case '-':
			target << uint8_t(0x11) << uint8_t(0x03) << uint8_t(0x01); //VSUB r3, r1; r3--
			break;
		case '.':
			target << uint8_t(0xF2) << uint8_t(0x03) << uint8_t(0x20); // VOUTB 0x20, r3; print byte from r3 to console
			break;
		case ',':
			target << uint8_t(0xF3) << uint8_t(0x03) << uint8_t(0x20); // VINB 0x20, r3; read byte from console to r3
			break;
		case '[':
			target << uint8_t(0x20) << uint8_t(0x03) << uint8_t(0x02); //VCMP r3, r2
			target << uint8_t(0x21) << uint16_t(0xFFFF); //if r3==0 then jump to end of loop
			loop_stack.push(target.tellp());
			pointer_counter.push(0);
			break;
		case ']':
			target << uint8_t(0x20) << uint8_t(0x03) << uint8_t(0x02); //VCMP r3, r2
			//there are computations of values needed to relative jumps to end and begin of loop
			target << uint8_t(0x22) << uint16_t((loop_stack.top() - target.tellp() - 3) % 0xFFFF); //if r3!=0 then jump to begin of loop
			auto current_pos = target.tellp();
			target.seekp(loop_stack.top() - 2);
			//set address of instruction after loop to jump instruction at the begin of loop
			target << uint16_t((current_pos - target.tellp() - 2) % 0xFFFF); 
			target.seekp(current_pos);
			loop_stack.pop();
			pointer_counter.pop();
		}
	}
	target << uint8_t(0xFF); //VOFF; turn of the virtual machine
	//here is block that handles NEGATIVE_POINTER_ERROR
	if (negative_pointer_threat)
	{
		auto current_pos = target.tellp();
		while (!decr_pointer_stack.empty())
		{
			target.seekp(decr_pointer_stack.top() - 2);
			//fill decrement pointer's jump instructions with address of this block
			target << uint16_t((current_pos - target.tellp() - 2) % 0xFFFF); 
			decr_pointer_stack.pop();
		}
		target.seekp(current_pos);
		auto write_error_text_jump = current_pos;
		//this prints error text
		target << uint8_t(0x01) << uint8_t(00) << uint32_t(0xFFFFFFFF); //set to r0 address of error text
		target << uint8_t(0x04) << uint8_t(0x03) << uint8_t(0x00); // VLDB r3, r0
		target << uint8_t(0x20) << uint8_t(0x02) << uint8_t(0x03); // VCMP r2, r3
		target << uint8_t(0x21) << uint16_t(0x0009); // jump to end if r0 == 0
		target << uint8_t(0xf2) << uint8_t(0x03) << uint8_t(0x20); // print byte from r3 to console
		target << uint8_t(0x10) << uint8_t(0x00) << uint8_t(0x01); // r0++
		target << uint8_t(0x40) << uint16_t(0xFFEE); // VJMP imm16 - jump to beginn of the loop
		target << uint8_t(0xFF); //VOFF; turn of the virtual machine
		current_pos = target.tellp();
		target.seekp(2 + write_error_text_jump);
		target << uint32_t(current_pos); //fill jump instruction with address of text
		target.seekp(current_pos);
		target << "NEGATIVE_POINTER_ERROR\n"; //error text
	}
	if (!loop_stack.empty())
	{
		m_error_list.push_back("SYNTAX ERROR: " + std::to_string(loop_stack.size()) + " loops never ended!");
		return false;
	}
	uint32_t memory_begin_address = target.tellp();
	target.seekp(memory_address_offset);
	target << memory_begin_address;	
	return true;
}