#include "Keyboard.h"
#include "Cell.h"

//#DEFINITION
///////////////////////////////////////////////// VARIABLES STATIQUES
 
int Keyboard::input = -1;

///////////////////////////////////////////////// METHODES

bool Keyboard::is_pressed(Key key) {

	switch (key)
	{
	case Keyboard::A:
		return input == 'a' || input == 'A';
		break;
	case Keyboard::B:
		return input == 'b' || input == 'B';
		break;
	case Keyboard::C:
		return input == 'c' || input == 'C';
		break;
	case Keyboard::D:
		return input == 'd' || input == 'D';
		break;
	case Keyboard::E:
		return input == 'e' || input == 'E';
		break;
	case Keyboard::F:
		return input == 'f' || input == 'F';
		break;
	case Keyboard::G:
		return input == 'g' || input == 'G';
		break;
	case Keyboard::H:
		return input == 'h' || input == 'H';
		break;
	case Keyboard::I:
		return input == 'i' || input == 'I';
		break;
	case Keyboard::J:
		return input == 'j' || input == 'J';
		break;
	case Keyboard::K:
		return input == 'k' || input == 'K';
		break;
	case Keyboard::L:
		return input == 'l' || input == 'L';
		break;
	case Keyboard::M:
		return input == 'm' || input == 'M';
		break;
	case Keyboard::N:
		return input == 'n' || input == 'N';
		break;
	case Keyboard::O:
		return input == 'o' || input == 'O';
		break;
	case Keyboard::P:
		return input == 'p' || input == 'P';
		break;
	case Keyboard::Q:
		return input == 'q' || input == 'Q';
		break;
	case Keyboard::R:
		return input == 'r' || input == 'R';
		break;
	case Keyboard::S:
		return input == 's' || input == 'S';
		break;
	case Keyboard::T:
		return input == 't' || input == 'T';
		break;
	case Keyboard::U:
		return input == 'u' || input == 'U';
		break;
	case Keyboard::V:
		return input == 'v' || input == 'V';
		break;
	case Keyboard::W:
		return input == 'w' || input == 'W';
		break;
	case Keyboard::X:
		return input == 'x' || input == 'X';
		break;
	case Keyboard::Y:
		return input == 'y' || input == 'Y';
		break;
	case Keyboard::Z:
		return input == 'z' || input == 'Z';
		break;
	case Keyboard::Num0:
		return input == 224 || input == '0'; //problème avec la reconnaissance de 'à' (meme avec toupper et tolowwer)
		break;
	case Keyboard::Num1:
		return input == '&' || input == '1';
		break;
	case Keyboard::Num2:
		return input == 233 || input == '2';  //problème avec la reconnaissance de 'é' (meme avec toupper et tolowwer)
		break;
	case Keyboard::Num3:
		return input == '\"' || input == '3';
		break;
	case Keyboard::Num4:
		return input == '\'' || input == '4';
		break;
	case Keyboard::Num5:
		return input == '(' || input == '5';
		break;
	case Keyboard::Num6:
		return input == '-' || input == '6';
		break;
	case Keyboard::Num7:
		return input == 232 || input == '7';  //problème avec la reconnaissance de 'è' (meme avec toupper et tolowwer)
		break;
	case Keyboard::Num8:
		return input == '_' || input == '8';
		break;
	case Keyboard::Num9:
		return input == 231 || input == '9';  //problème avec la reconnaissance de 'ç' (meme avec toupper et tolowwer)
		break;
	case Keyboard::Escape:
		return input == 27; //Surement probleme de compatibilité
		break;
	case Keyboard::SemiColon:
		return input == ';' || input == '.';
		break;
	case Keyboard::Comma:
		return input == ',' || input == '?';
		break;
	case Keyboard::Period:
		return input == '<' || input == '>';
		break;
	case Keyboard::Equal:
		return input == '=' || input == '+';
		break;
	case Keyboard::Space:
		return input == ' ';
		break;
	case Keyboard::Return:
		return input == KEY_ENTER || input == 13 || input == 10; //Surement probleme de compatibilité
		break;
	case Keyboard::BackSpace:
		return input == KEY_BACKSPACE || input == 8; //Surement probleme de compatibilité
		break;
	case Keyboard::Tab:
		return input == KEY_BTAB || input == 9; //Surement probleme de compatibilité
		break;
	case Keyboard::Left:
		return input == KEY_LEFT;
		break;
	case Keyboard::Right:
		return input == KEY_RIGHT;
		break;
	case Keyboard::Up:
		return input == KEY_UP;
		break;
	case Keyboard::Down:
		return input == KEY_DOWN;
		break;
	case Keyboard::Numpad0:
		return input == '0';
		break;
	case Keyboard::Numpad1:
		return input == '1';
		break;
	case Keyboard::Numpad2:
		return input == '2';
		break;
	case Keyboard::Numpad3:
		return input == '3';
		break;
	case Keyboard::Numpad4:
		return input == '4';
		break;
	case Keyboard::Numpad5:
		return input == '5';
		break;
	case Keyboard::Numpad6:
		return input == '6';
		break;
	case Keyboard::Numpad7:
		return input == '7';
		break;
	case Keyboard::Numpad8:
		return input == '8';
		break;
	case Keyboard::Numpad9:
		return input == '9';
		break;
	case Keyboard::F1:
		return input == KEY_F(1);
		break;
	case Keyboard::F2:
		return input == KEY_F(2);
		break;
	case Keyboard::F3:
		return input == KEY_F(3);
		break;
	case Keyboard::F4:
		return input == KEY_F(4);
		break;
	case Keyboard::F5:
		return input == KEY_F(5);
		break;
	case Keyboard::F6:
		return input == KEY_F(6);
		break;
	case Keyboard::F7:
		return input == KEY_F(7);
		break;
	case Keyboard::F8:
		return input == KEY_F(8);
		break;
	case Keyboard::F9:
		return input == KEY_F(9);
		break;
	case Keyboard::F10:
		return input == KEY_F(10);
		break;
	case Keyboard::F11:
		return input == KEY_F(11);
		break;
	case Keyboard::F12:
		return input == KEY_F(12);
		break;
	default:
		return false;
		break;
	}
}

//#DEFINITION_END