/* some useful vim command
:let @x='yy@"j'
j7@x
:map <C-b> :wa<CR>:!make backup<CR>
:map <C-d> :wa<CR>:!make dump<CR>
:map <C-m> :wa<CR>:!make clean build<CR>
:map <C-p> :wa<CR>:!make all<CR>
:let @c="0i//\ej"
:let @u='02xj'
:let @d="oDEBUG1; //TODO\e"
*/

/*
 * main program of the display controller
 * @author haggl
 * @version 0.2.2 (4): 2011-09-04
*/

#include "ctrldog.h" 


int main( void )
{
	init();

	// main program
	while (1)
	{
		switch ( action() )
		{
			case CTRL_OK_ACTION:
				menu();
				break;

			case CTRL_DN_ACTION:
				COUNT_DN
				enterProgram( program.number );
				break;

			case CTRL_UP_ACTION:
				COUNT_UP
				enterProgram( program.number );
				break;

			case COM_ACTION:
				execCMD( receive_com( 10 ) );
				break;

			default:
				continue;
		}
	}

	return 0;
}
