#include "pch.h"
//
namespace SeAn
{
	 Error::ERROR temperr;// ���������� ��� �������� ������� ������
	short SeAn::CheckingReturnInMain(LT::LexTable& lextable, IT::IdTable& idtable,Log::LOG logfile)
	{
		short errExist = 0;//// ������� ������ 
		short nestingLevel = 0;//������� �����������
		short warmingID = 0;//1 - �� ���� ��� ���������� ��������(�������� return ������ � if �����)  // ��������� ������ � ��������������
		for (int lexema_I = 0; lexema_I < lextable.size; lexema_I++)// ������� ������� ������
		{
			if (lextable.table[lexema_I].lexeme == LEX_MAIN)// ���� ������� ����� ����� (main)
			{
				for (int lexIndexInMain = lexema_I; lexIndexInMain < lextable.size; lexIndexInMain++)// ����� ����������� main
				{
					if (lextable.table[lexIndexInMain].lexeme == LEX_LEFT_SQUARE_BRACE)
					{
						nestingLevel++;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_RIGHT_SQUARE_BRACE)
					{
						nestingLevel--;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_FUNCTION)// ���� ������ main ���� ���������� �������
					{
						errExist++;// ������, ������� � main ���������
						 temperr = Error::geterrorin(702, lextable.table[lexema_I].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_RETURN)
					{
						
						for ( ; lexIndexInMain <lextable.size&&lextable.table[lexIndexInMain].lexeme!=LEX_SEMICOLON; lexIndexInMain++)//���� �� ����� �������
						{
							if (lextable.table[lexIndexInMain].idxTI != LT_TI_NULLIDX && idtable.table[lextable.table[lexIndexInMain].idxTI].iddatatype !=IT::UINT)
							{
								errExist++;
								 temperr = Error::geterrorin(703, lextable.table[lexema_I].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
							}//���� � ����� ���������� �� INT
						}	

						if (nestingLevel == 0)
						{
							warmingID = -1;//������������� ���������� ��������
						}
						else
							warmingID = 1;
					}
					
				}
				if (warmingID == 1) // �������� �� ���������� return
				{
					errExist++;
					 temperr = Error::geterrorin(701, lextable.table[lexema_I].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
				}//warming - �� ���� ��� ���������� return
				if (warmingID == 0)//���� ���������� �� ���������
				{
					errExist++;
					 temperr = Error::geterrorin(700, lextable.table[lexema_I].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
				}//warming - ��� return

				break;
			}
			
		}
		return errExist;
	}
	// �������� ������� return � ���������������� ��������
	short SeAn::CheckReturnInUserFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile)
	{
		short errExist = 0;
		for (int i = 0; i < lextable.size; i++)
		{//���� ���������������� �������
			if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F 
				&& lextable.table[i-1].lexeme == LEX_FUNCTION)
			{
				//��������� ������� ��� �������
				IT::IDDATATYPE CurrentTypeOfFunction = idtable.table[lextable.table[i].idxTI].iddatatype;//�������� � ����� ��� � �������� ���
				
				//����� �����,������� ��������� � ��������� � ���� �������
				while (lextable.table[i].lexeme != LEX_LEFTBRACE && i < lextable.size)
				{
					i++;
				}
				short nestingLevel = 0; 
				short warmingID = 0;
				//WARMING ID:
				//-1 - return ������ ���������� ��������
				//0 - return ����������� � �������
				//1 - return �� ������ ���������� ��������
				for (; i < lextable.size && lextable.table[i].lexeme!=LEX_BRACELET; i++)
				{
					//��������� �� ����������� 
					if (lextable.table[i].lexeme == LEX_LEFT_SQUARE_BRACE)
					{
						nestingLevel++;
					}
					if (lextable.table[i].lexeme == LEX_RIGHT_SQUARE_BRACE)
					{
						nestingLevel--;
					}
					if (lextable.table[i].lexeme == LEX_RETURN)
					{
						//��������� ��� ������������� ��������
						for (; i < lextable.size && lextable.table[i].lexeme != LEX_SEMICOLON; i++)
						{
							if (lextable.table[i].idxTI != LT_TI_NULLIDX && idtable.table[lextable.table[i].idxTI].iddatatype != CurrentTypeOfFunction)
							{
								errExist++;
								 temperr = Error::geterrorin(704, lextable.table[i].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
							}//���� ����������� �������� �� ������������� ���
						}
						if (nestingLevel != 0)
							warmingID = 1;
						if (nestingLevel == 0)
							warmingID = -1;
					}
				}
				switch (warmingID)
				{
				case -1:
					break;
				case 0:
					errExist++;
					 temperr = Error::geterrorin(700, lextable.table[i].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//���� �������
					break;
				case 1:
					errExist++;
					 temperr = Error::geterrorin(701, lextable.table[i].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//���� �������
					break;
				default:
					break;
				}
			}
		}
		return errExist;
	}

	short SeAn::CheckParamsOfFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile)
	{
		short errExist = 0;
		for (int i = 0; i < lextable.size; i++)
		{
			//���� ����� ���������������� �������
			if (lextable.table[i].lexeme == LEX_ID && lextable.table[i - 1].lexeme != LEX_FUNCTION &&
				idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F)
			{
				int currentFuncIndex = i;//���������� ������� ����������� �������
				int currentIndexParam = 0;//����� ���������
				i++;
				//��������� �� ����� ������ �������
				while (lextable.table[i].lexeme!=LEX_RIGHTHESIS && i < lextable.size)
				{
					// ���� ��� ������������� ��� �������
					if (lextable.table[i].lexeme == LEX_ID || (lextable.table[i].idxTI!=TI_NULLIDX&&
						idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L))
					{
					
						
						if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount <= currentIndexParam)
						{
							errExist++;
							 temperr = Error::geterrorin(705, lextable.table[i].sn, -1);
							*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
						}//���� ���������� ������ ��� ���������� � �������
							//�������� �� ������������ ����� ���������� � �����������
							if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.types[currentIndexParam] !=
								idtable.table[lextable.table[i].idxTI].iddatatype
								)
							{
								errExist++;
								 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//��� ��������� �� ������������� ���� ���������
							}
							currentIndexParam++;
						
					}
					i++;
				}
				if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount != currentIndexParam)
				{
					errExist++;
					 temperr = Error::geterrorin(707, lextable.table[i].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
				}//������� ���� ���������� ���������
			}
		}
		return errExist;
	}
	
	short SeAn::CheckParamsStdFunc(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile)//���� ����
	{
		short errExist = 0;
		for (int i = 0; i < lextable.size; i++)
		{
			if (SeAn::FindSTD(lextable.table[i].lexeme) && lextable.table[i - 1].lexeme == LEX_FUNCTION)
			{
				errExist++;
				 temperr = Error::geterrorin(708, lextable.table[i].sn, -1);
				*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
			}//������� �������������� ������������ �������

			if (lextable.table[i].lexeme == LEX_DIRSLASH)
			{
				int divisionIndex = i;

				i++; 
				if (lextable.table[i].lexeme == LEX_LITERAL)
				{
					if (idtable.table[lextable.table[i].idxTI].value.vint == 0)
					{
						errExist++;
						temperr = Error::geterrorin(712, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ") " << temperr.message << " � ������ " << temperr.inext.line << std::endl;
					}
				}
				else if (lextable.table[i].lexeme == LEX_ID)
				{
					if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::UINT &&
						idtable.table[lextable.table[i].idxTI].value.vint == 0)
					{
						errExist++;
						temperr = Error::geterrorin(713, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ") " << temperr.message << " � ������ " << temperr.inext.line << std::endl;
					}
				}
			}


			if (SeAn::FindSTD(lextable.table[i].lexeme))
			{
			
				short amountParams = 0;

				switch (lextable.table[i].lexeme)
				{
				
				case LEX_RAND:
					i++;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						if (lextable.table[i].lexeme == LEX_ID)
							if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
							{
								errExist++;
								 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
							}//��� ���������� �� ������������ ���� ���������
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
								{
									errExist++;
									 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
									*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//��� �������� �� ������������� ���� ���������
								}
								else {
									amountParams++;
								}
						i++;
					}
					if (amountParams < 1)
					{
						errExist++;
						 temperr = Error::geterrorin(707, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
					}//������� ���� ����������
					else if (amountParams > 1)
					{
						errExist++;
						 temperr = Error::geterrorin(705, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//������� �����
					}

					break;
				case LEX_POW:
					i++;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						if (lextable.table[i].lexeme == LEX_ID)
							if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
							{
								errExist++;
								 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
								*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
							}//��� ���������� �� ������������ ���� ���������
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
								{
									errExist++;
									 temperr = Error::geterrorin(706, lextable.table[i].sn, -1);
									*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//��� �������� �� ������������� ���� ���������
								}
								else {
									amountParams++;
								}
						i++;
					}
					if (amountParams < 2)
					{
						errExist++;
						 temperr = Error::geterrorin(707, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
					}//������� ���� ����������
					else if (amountParams > 2)
					{
						errExist++;
						 temperr = Error::geterrorin(705, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//������� �����
					}

					break;
				case LEX_PRINT:
				{
					i += 2;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						amountParams++;
						i++;
					}
					if (amountParams < 1)
					{
						errExist++;
						 temperr = Error::geterrorin(707, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
					}//������� ���� ����������
					else if (amountParams > 1)
					{
						errExist++;
						 temperr = Error::geterrorin(705, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//������� �����
					}
					break;
				}
				default:
					errExist++;
					 temperr = Error::geterrorin(709, lextable.table[i].sn, -1);
					*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//����������� ������������ �������
					break;
				}
			}
		}
		return errExist;
	}
	bool SeAn::FindSTD(unsigned char lex)
	{
		switch (lex)
		{
		case LEX_RAND:
			return true;
		case LEX_POW:
			return true;
		case LEX_PRINT:
			return true;
		default:
			return false;
			break;
		}
	}


	short SeAn::Types(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG logfile)
	{
		short errExist = 0;
		IT::IDDATATYPE currentType;//������� ���


		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexeme == LEX_EQUALS )
			{
				currentType = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
				while (lextable.table[i].lexeme!=LEX_SEMICOLON && i<lextable.size)
				{
					i++;
					if ((lextable.table[i].lexeme == LEX_ID || lextable.table[i].lexeme == LEX_LITERAL)
						&& idtable.table[lextable.table[i].idxTI].iddatatype != currentType)
					{
						if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F ||
							idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::S)
						{
							errExist++;
							 temperr = Error::geterrorin(710, lextable.table[i].sn, -1);
							*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
						}//������� ���������� �� ��� ��� ������
						else if(idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::V ||
							idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
						{
							errExist++;
							 temperr = Error::geterrorin(710, lextable.table[i].sn, -1);
							*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
						}//�� ������ ��� ������ ������������
					}
					else if (lextable.table[i].lexeme == LEX_ID
						&& (idtable.table[lextable.table[i].idxTI].idtype != IT::IDTYPE::F&&
							idtable.table[lextable.table[i].idxTI].idtype != IT::IDTYPE::S)
						&& lextable.table[i + 1].lexeme == LEX_LEFTHESIS)
					{
						errExist++;
						 temperr = Error::geterrorin(711, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;//�� ������ ����� �������
					}
					else if (lextable.table[i].lexeme == LEX_RAND && currentType != IT::IDDATATYPE::UINT)
					{
						errExist++;
						 temperr = Error::geterrorin(710, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
					}
					else if (lextable.table[i].lexeme == LEX_POW && currentType != IT::IDDATATYPE::UINT)
					{
						errExist++;
						 temperr = Error::geterrorin(710, lextable.table[i].sn, -1);
						*logfile.stream << '(' << temperr.id << ')' << temperr.message << " � ������ " << temperr.inext.line << std::endl;
					}
					if (SeAn::FindSTD(lextable.table[i].lexeme) ||
						lextable.table[i].lexeme == LEX_FUNCTION)
					{
						while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
							i++;
					}
				}
			}
		}
		return errExist;
	}
	
}

