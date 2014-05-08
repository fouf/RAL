#include "sim.h"
#include <commctrl.h>
extern app *Application;
extern time *Gametimer;
int CALLBACK Dialog_Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void simulation_newgeneration();
void setStatus(char *msg)
{
	SendDlgItemMessage(simulation.Config, IDC_STATUS, LB_INSERTSTRING, 0, (LPARAM)msg);
}

void init_dialog()
{
	simulation.Config = CreateDialog(Application->hInst, MAKEINTRESOURCE(IDD_DIALOG1), Application->hWnd, Dialog_Proc);
	ShowWindow(simulation.Config, SW_SHOW);
	char buff[512]; snprintf(buff, 512, "Area: %d x %d", MAP_WIDTH, MAP_HEIGHT);
	SetDlgItemText(simulation.Config, IDC_TEXTAREA, buff);
	SetDlgItemText(simulation.Config, IDC_TIMEDELAY, "1");
	char width[10], height[10]; itoa(MAP_HEIGHT, height, 10); itoa(MAP_WIDTH, width, 10);
	SetDlgItemText(simulation.Config, IDC_WIDTH, width);
	SetDlgItemText(simulation.Config, IDC_HEIGHT, height);
	SetDlgItemText(simulation.Config, IDC_GENERATIONTICK, "1");
	itoa(Application->r_h, height, 10); itoa(Application->r_w, width, 10);
	SetDlgItemText(simulation.Config, IDC_SCREENWIDTH, width);
	SetDlgItemText(simulation.Config, IDC_SCREENHEIGHT, height);

	SendDlgItemMessage(simulation.Config, IDC_PRED_BREED, TBM_SETPOS, 1, simulation.g_pred_breed);
	snprintf(buff, 512, "%d", simulation.g_pred_breed);
	SetDlgItemText(simulation.Config, IDC_PRED_BREED_V, buff);
	
	SendDlgItemMessage(simulation.Config, IDC_PRED_STARVE, TBM_SETPOS, 1, simulation.g_pred_starve);
	snprintf(buff, 512, "%d", simulation.g_pred_starve);
	SetDlgItemText(simulation.Config, IDC_PRED_STARVE_V, buff);
	
	SendDlgItemMessage(simulation.Config, IDC_PREY_BREED, TBM_SETPOS, 1, simulation.g_prey_breed);
	snprintf(buff, 512, "%d", simulation.g_prey_breed);
	SetDlgItemText(simulation.Config, IDC_PREY_BREED_V, buff);
	
}
int CALLBACK Dialog_Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char buf[10];
	switch(Message)
	{
		case WM_CLOSE:
			DestroyWindow(simulation.Config);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_PAUSE:
			 		if(Gametimer->t_paused)
					{
						time_unpause(Gametimer);
						SetDlgItemText(simulation.Config, ID_PAUSE, "Pause");
						SDL_WM_SetCaption("PvP", NULL);
						setStatus("PvP unpaused.");
					}
					else
					{
						time_pause(Gametimer);
						SetDlgItemText(simulation.Config, ID_PAUSE, "Unpause");
						SDL_WM_SetCaption("PvP - Paused", NULL);
						setStatus("PvP paused.");
					}
				break;
				case ID_STOP:
					time_unpause(Gametimer);
					simulation.state = 0;
					Application->clear = 1;
					simulation_cleanup();
					DestroyWindow(simulation.Config);	
				break;
				case ID_CLEAR:
					simulation_cleanup();
					cleanup_cells();
					simulation_render(0.01f);
					
				break;
				case IDC_UPDATETIME:
					GetDlgItemText(simulation.Config, IDC_TIMEDELAY, buf, 15);
					int delay = atoi(buf);
					if(delay >= 0 && delay < 10000)
					{
						simulation.tickdelay = delay;
					}
					GetDlgItemText(simulation.Config, IDC_GENERATIONTICK, buf, 15);
					int genpertick = atoi(buf);
					if(genpertick > 0)
					{		
						simulation.generations = genpertick;
					}
				break;
				case IDC_UPDATEAREA:
					GetDlgItemText(simulation.Config, IDC_WIDTH, buf, 15);
					int width = atoi(buf);
					if(width > 0 && width < 3500)
					{
						MAP_WIDTH = width;
					}
					GetDlgItemText(simulation.Config, IDC_HEIGHT, buf, 15);
					int height = atoi(buf);
					if(height > 0 && height < 3500)
					{
						MAP_HEIGHT = height;
					}
					GetDlgItemText(simulation.Config, IDC_SCREENWIDTH, buf, 15);
					width = atoi(buf);
					if(width > 0)
					{
						Application->r_w = width;
					}
					GetDlgItemText(simulation.Config, IDC_SCREENHEIGHT, buf, 15);
					height = atoi(buf);
					if(height > 0)
					{
						Application->r_h = height;
					}
					DestroyWindow(simulation.Config);
					simulation.Config = NULL;
					Application->Window = SDL_SetVideoMode(width, height + GRAPH_HEIGHT, 24, Application->Flags);
					simulation_init();					
					break;
					
				case ID_PICTURE:
					SDL_SaveBMP(Application->Window, "Screenshot.bmp");
				break;
			}
		break;
		default:
		return 0;
	}
	return 1;
}
