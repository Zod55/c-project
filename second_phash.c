/* need to implement all the table retrive the file and go over the lines one by one
in the new file i dont need to have the labels since i dont really care about them 
i have the label_list and if it not there it is extern and the line will be 0x00000001
*/
/*first trim the line then check what is the command 
then check how many register we will use in it
thee check for num of register per command will be checked in the parser (cant put register in bne or jmp etc.) */

void code_generation(){
	/*if we dont have any entry we dont need to make any file for it*/
   if(entry_list != NULL)
	   make_entry_file();
   /*same here for the extern*/
   if(extern_list != NULL)
	make_extern_file();
   /*here we would generate the code*/
   file_code_generation();

}
/*we could say this is the main function we will check for each line if it have one,two or zero labels and then 
 * if it is zero labels we would havbe one line of hex values
 * 1 laebl we would have 2 lines 
 * 2 we would have 3 lines
 * 3 function for handling everything */
void file_code_generation()
{
}



