#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>

#define LICZBA_POL 22 //bok
#define LICZBA_MIN 16
#define ROZMIAR_POLA 40
#define MARGINES_POLA 2


struct pole {
	bool mina;
	bool zakryte;
	bool oznaczone;
	unsigned short int ileObok;
};

void init_allegro();
void wypelnij_pola();
void rysuj_pola();
void oznacz_pole();
bool odkryj_pole();
void odkryj_poboczne();

int main(int argc, char * argv[])
{	
	srand(time(NULL));

	struct pole pola[LICZBA_POL][LICZBA_POL];


	ALLEGRO_DISPLAY * display = NULL;

	ALLEGRO_EVENT_QUEUE * kolejka = NULL;

	ALLEGRO_EVENT ev1;

	ALLEGRO_FONT * font = NULL;
	ALLEGRO_FONT * bigFont = NULL;


	init_allegro();

	kolejka = al_create_event_queue();
	if (kolejka == NULL)
	{
		fprintf(stderr, "Failed to create event queue!\n");
		return -1;
	}

	display = al_create_display(400, 400);
	if (display == NULL)
	{
		fprintf(stderr, "Failed to create display!\n");
		return -1;
	}

	font = al_load_ttf_font("OpenSans-Regular.ttf", 28, 0);
	if (font == NULL)
	{
		fprintf(stderr, "Failed to load font!\n");
		return -1;
	}

	bigFont = al_load_ttf_font("OpenSans-Regular.ttf", 36, 0);
	if (bigFont == NULL)
	{
		fprintf(stderr, "Failed to load font!\n");
		return -1;
	}

	al_register_event_source(kolejka, al_get_display_event_source(display));
	al_register_event_source(kolejka, al_get_mouse_event_source());
	al_register_event_source(kolejka, al_get_keyboard_event_source());
	
	

	int liczbaPol = (double)LICZBA_POL*0.6;
	int liczbaMin = (double)liczbaPol*(double)liczbaPol*0.2;
	int wysOkna = liczbaPol * ROZMIAR_POLA;
	int szerOkna = liczbaPol * ROZMIAR_POLA;
	al_resize_display(display, liczbaPol*ROZMIAR_POLA, liczbaPol*ROZMIAR_POLA);




	while (1)
	{
		wypelnij_pola(liczbaPol, liczbaMin, pola);
		short int statusGry = 0;

		int liczbaZakrytych = liczbaPol*liczbaPol;

		while (statusGry == 1 || statusGry==0) {

			al_wait_for_event(kolejka, &ev1);

			if (ev1.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				return 0;
			}

			if (ev1.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				printf("klik %i, %i \n", ev1.mouse.x, ev1.mouse.y);
				int i = ev1.mouse.x / ROZMIAR_POLA;
				int j = ev1.mouse.y / ROZMIAR_POLA;
				if (ev1.mouse.button == 1) {
					do {
						if (odkryj_pole(i, j, liczbaPol, pola, &liczbaZakrytych)) {
							statusGry = 1;
							if (liczbaZakrytych <= liczbaMin) {
								statusGry = 3;
							}
						}
						else {//GAME OVER
							if (statusGry == 0) {
								wypelnij_pola(liczbaPol, liczbaMin, pola);
							}
							else {
								statusGry = 2;
							}
						}
					} while (statusGry == 0);
				}
				else if (ev1.mouse.button == 2) {
					oznacz_pole(i, j, liczbaPol, pola);
				}
			}
			if (ev1.type == ALLEGRO_EVENT_KEY_CHAR) {
				printf("wcisnieto %i \n", ev1.keyboard.keycode);
				if (ev1.keyboard.keycode == 18) {//r
					statusGry = 4;
				}
				else if (ev1.keyboard.keycode == 28) {//1
					liczbaPol = (double)LICZBA_POL * 0.6;
					liczbaMin = (double)liczbaPol*(double)liczbaPol * 0.2;
					wysOkna = liczbaPol * ROZMIAR_POLA;
					szerOkna = liczbaPol * ROZMIAR_POLA;
					al_resize_display(display, liczbaPol*ROZMIAR_POLA, liczbaPol*ROZMIAR_POLA);
					bigFont = al_load_ttf_font("OpenSans-Regular.ttf", 36, 0);
					if (bigFont == NULL)
					{
						fprintf(stderr, "Failed to load font!\n");
						return -1;
					}
					statusGry = 4;
					wypelnij_pola(liczbaPol, liczbaMin, pola);

				}
				else if (ev1.keyboard.keycode == 29) {//2
					liczbaPol = (double)LICZBA_POL * 0.8;
					liczbaMin = (double)liczbaPol*(double)liczbaPol * 0.2;
					wysOkna = liczbaPol * ROZMIAR_POLA;
					szerOkna = liczbaPol * ROZMIAR_POLA;
					al_resize_display(display, liczbaPol*ROZMIAR_POLA, liczbaPol*ROZMIAR_POLA);
					bigFont = al_load_ttf_font("OpenSans-Regular.ttf", 46, 0);
					if (bigFont == NULL)
					{
						fprintf(stderr, "Failed to load font!\n");
						return -1;
					}
					statusGry = 4;
					wypelnij_pola(liczbaPol, liczbaMin, pola);

				}
				else if (ev1.keyboard.keycode == 30) {//3
					liczbaPol = (double)LICZBA_POL * 1;
					liczbaMin = (double)liczbaPol*(double)liczbaPol * 0.2;
					wysOkna = liczbaPol * ROZMIAR_POLA;
					szerOkna = liczbaPol * ROZMIAR_POLA;
					al_resize_display(display, liczbaPol*ROZMIAR_POLA, liczbaPol*ROZMIAR_POLA);
					bigFont = al_load_ttf_font("OpenSans-Regular.ttf", 66, 0);
					if (bigFont == NULL)
					{
						fprintf(stderr, "Failed to load font!\n");
						return -1;
					}
					statusGry = 4;
					wypelnij_pola(liczbaPol, liczbaMin, pola);

				}

			}
			rysuj_pola(liczbaPol, pola, font);
			al_flip_display();
		}
		
		if (statusGry == 2) {
			al_draw_text(bigFont, al_map_rgb(6, 6, 6), szerOkna / 2, wysOkna*0.2, ALLEGRO_ALIGN_CENTER, "BOOOM!");
			al_draw_text(bigFont, al_map_rgb(6, 6, 6), szerOkna / 2, wysOkna*0.4, ALLEGRO_ALIGN_CENTER, "PRZEGRANA!");
		}
		else if(statusGry==3){
			al_draw_text(bigFont, al_map_rgb(6, 6, 6), szerOkna / 2, wysOkna*0.2, ALLEGRO_ALIGN_CENTER, "WYGRANA!");
		}
			al_flip_display();

		while (statusGry!=4) {
			al_wait_for_event(kolejka, &ev1);

			if (ev1.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				statusGry=4;
			}
			if (ev1.type == ALLEGRO_EVENT_KEY_CHAR) {
				printf("wcisnieto %i \n", ev1.keyboard.keycode);
				if (ev1.keyboard.keycode == 18) {//r
					statusGry = 4;
				}
				else if (ev1.keyboard.keycode == 28) {//1
					liczbaPol = (double)LICZBA_POL * 0.6;
					liczbaMin = (double)liczbaPol*(double)liczbaPol * 0.2;
					wysOkna = liczbaPol * ROZMIAR_POLA;
					szerOkna = liczbaPol * ROZMIAR_POLA;
					al_resize_display(display, liczbaPol*ROZMIAR_POLA, liczbaPol*ROZMIAR_POLA);
					bigFont = al_load_ttf_font("OpenSans-Regular.ttf", 36, 0);
					if (bigFont == NULL)
					{
						fprintf(stderr, "Failed to load font!\n");
						return -1;
					}
					statusGry = 4;
					wypelnij_pola(liczbaPol, liczbaMin, pola);

				}
				else if (ev1.keyboard.keycode == 29) {//2
					liczbaPol = (double)LICZBA_POL * 0.8;
					liczbaMin = (double)liczbaPol*(double)liczbaPol * 0.2;
					wysOkna = liczbaPol * ROZMIAR_POLA;
					szerOkna = liczbaPol * ROZMIAR_POLA;
					al_resize_display(display, liczbaPol*ROZMIAR_POLA, liczbaPol*ROZMIAR_POLA);
					bigFont = al_load_ttf_font("OpenSans-Regular.ttf", 46, 0);
					if (bigFont == NULL)
					{
						fprintf(stderr, "Failed to load font!\n");
						return -1;
					}
					statusGry = 4;
					wypelnij_pola(liczbaPol, liczbaMin, pola);

				}
				else if (ev1.keyboard.keycode == 30) {//3
					liczbaPol = (double)LICZBA_POL * 1;
					liczbaMin = (double)liczbaPol*(double)liczbaPol * 0.2;
					wysOkna = liczbaPol * ROZMIAR_POLA;
					szerOkna = liczbaPol * ROZMIAR_POLA;
					al_resize_display(display, liczbaPol*ROZMIAR_POLA, liczbaPol*ROZMIAR_POLA);
					bigFont = al_load_ttf_font("OpenSans-Regular.ttf", 66, 0);
					if (bigFont == NULL)
					{
						fprintf(stderr, "Failed to load font!\n");
						return -1;
					}
					statusGry = 4;
					wypelnij_pola(liczbaPol, liczbaMin, pola);

				}

			}
		}
	}
	return 0;
}


void init_allegro() {
	if (!al_init())
	{
		fprintf(stderr, "Failed to initialize allegro!\n");
		return -1;
	}

	if (!al_init_font_addon()) {
		fprintf(stderr, "Couldn't initialize fonts addon!\n");
		return -1;
	}

	if (!al_init_ttf_addon()) {
		fprintf(stderr, "Couldn't initialize ttf addon!\n");
		return -1;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "Couldn't initialize primitives addon!\n");
		return -1;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "Couldn't install mouse!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "Couldn't install keyboard!\n");
		return -1;
	}

}

void wypelnij_pola(int liczbaPol, int liczbaMin, struct pole pola[LICZBA_POL][LICZBA_POL]) {
	for (int i = 0; i < liczbaPol; ++i) {
		for (int j = 0; j < liczbaPol; ++j) {
			pola[i][j].zakryte = true;
			pola[i][j].oznaczone = false;
			pola[i][j].mina = false;
			pola[i][j].ileObok = 0;
		}
	}

	unsigned int x, y;
	for (int i = 0; i < liczbaMin;) {
		x = rand() % (liczbaPol);
		y = rand() % (liczbaPol);

		if (pola[x][y].mina == true) {
			continue;
		}
		else {
			pola[x][y].mina = true;
			if (x > 0) ++pola[x - 1][y].ileObok;
			if (y > 0) ++pola[x][y - 1].ileObok;

			if (x > 0 && y > 0) ++pola[x - 1][y - 1].ileObok;
			if (x < (liczbaPol - 1) && y < (liczbaPol - 1)) ++pola[x + 1][y + 1].ileObok;

			if (x < liczbaPol - 1) ++pola[x + 1][y].ileObok;
			if (y < liczbaPol - 1) ++pola[x][y + 1].ileObok;

			if (x < (liczbaPol - 1) && y > 0) ++pola[x + 1][y - 1].ileObok;
			if (x > 0 && y < (liczbaPol - 1)) ++pola[x - 1][y + 1].ileObok;

			++i;
		}
	}



}

void rysuj_pola(int liczbaPol, struct pole pola[LICZBA_POL][LICZBA_POL], ALLEGRO_FONT * font) {
	al_clear_to_color(al_map_rgb(60, 60, 240));
	//al_flip_display();
	for (int i = 0; i < liczbaPol; ++i) {
		for (int j = 0; j < liczbaPol; ++j) {
			if (pola[i][j].zakryte == true) {
				al_draw_filled_rectangle(ROZMIAR_POLA*i + MARGINES_POLA, ROZMIAR_POLA*j + MARGINES_POLA, ROZMIAR_POLA * i + ROZMIAR_POLA - MARGINES_POLA, ROZMIAR_POLA * j + ROZMIAR_POLA - MARGINES_POLA, al_map_rgb(160, 160, 240));
				if (pola[i][j].oznaczone == true) {
					al_draw_text(font, al_map_rgb(6, 6, 6), ROZMIAR_POLA*i + (ROZMIAR_POLA - MARGINES_POLA) / 2, ROZMIAR_POLA*j, ALLEGRO_ALIGN_CENTER, "&");
				}
			}
			else {
				if (pola[i][j].mina == true) {
					al_draw_filled_rectangle(ROZMIAR_POLA*i + MARGINES_POLA, ROZMIAR_POLA*j + MARGINES_POLA, (ROZMIAR_POLA * (i + 1)) - MARGINES_POLA, (ROZMIAR_POLA * (j + 1)) - MARGINES_POLA, al_map_rgb(240, 240, 240));
					al_draw_text(font, al_map_rgb(6, 6, 6), ROZMIAR_POLA*i + (ROZMIAR_POLA - MARGINES_POLA) / 2, ROZMIAR_POLA*j, ALLEGRO_ALIGN_CENTER, "*");
				}
				else {
					al_draw_filled_rectangle(ROZMIAR_POLA*i + MARGINES_POLA, ROZMIAR_POLA*j + MARGINES_POLA, (ROZMIAR_POLA * (i + 1)) - MARGINES_POLA, (ROZMIAR_POLA * (j + 1)) - MARGINES_POLA, al_map_rgb(240, 240, 240));
					if (pola[i][j].ileObok != 0) {
						char znak[2] = "";
						znak[0] = 48 + pola[i][j].ileObok;
						al_draw_text(font, al_map_rgb(6, 6, 6), ROZMIAR_POLA*i + (ROZMIAR_POLA - MARGINES_POLA) / 2, ROZMIAR_POLA*j, ALLEGRO_ALIGN_CENTER, &znak);
					}
				}
			}
		}
	}
}

bool odkryj_pole(int i, int j, int liczbaPol,struct pole pola[LICZBA_POL][LICZBA_POL], int *liczbaZakrytych) {
	
	if (i < liczbaPol&&j < liczbaPol) {
		if (pola[i][j].oznaczone == false && pola[i][j].zakryte == true) {
			printf("Odkryj  %i, %i \n", i, j);
			pola[i][j].zakryte = false;
			--*liczbaZakrytych;
			if(!pola[i][j].mina)
				odkryj_poboczne(i, j, liczbaPol, pola, liczbaZakrytych);
			return !pola[i][j].mina;
		}
	}
	return true;	
}

void oznacz_pole(int i, int j, int liczbaPol, struct pole pola[LICZBA_POL][LICZBA_POL]) {
	if (i < liczbaPol&&j < liczbaPol) {
		if (pola[i][j].zakryte == true) {
			printf("Oznacz  %i, %i \n", i, j);
			pola[i][j].oznaczone = !pola[i][j].oznaczone;
		}
	}
}

void odkryj_poboczne(int i, int j, int liczbaPol, struct pole pola[LICZBA_POL][LICZBA_POL], int *liczbaZakrytych) {
	if (pola[i][j].ileObok != 0) {
		if (i > 0 && pola[i - 1][j].ileObok == 0 && pola[i - 1][j].mina == false)odkryj_pole(i - 1, j, liczbaPol, pola, liczbaZakrytych);
		if (j > 0 && pola[i][j - 1].ileObok == 0 && pola[i][j - 1].mina == false)odkryj_pole(i, j - 1, liczbaPol, pola, liczbaZakrytych);

		if (i < (liczbaPol - 1) && pola[i + 1][j].ileObok == 0 && pola[i + 1][j].mina == false)odkryj_pole(i + 1, j, liczbaPol, pola, liczbaZakrytych);
		if (j < (liczbaPol - 1) && pola[i][j + 1].ileObok == 0 && pola[i][j + 1].mina == false)odkryj_pole(i, j + 1, liczbaPol, pola, liczbaZakrytych);
	}
	else {
		if (i > 0  && pola[i - 1][j].mina == false)odkryj_pole(i - 1, j, liczbaPol, pola, liczbaZakrytych);
		if (j > 0 && pola[i][j - 1].mina == false)odkryj_pole(i, j - 1, liczbaPol, pola, liczbaZakrytych);

		if (i > 0 && j > 0 && pola[i - 1][j - 1].mina == false)odkryj_pole(i - 1, j - 1, liczbaPol, pola, liczbaZakrytych);


		if (i < (liczbaPol - 1) &&  pola[i + 1][j].mina == false)odkryj_pole(i + 1, j, liczbaPol, pola,liczbaZakrytych);
		if (j < (liczbaPol - 1) &&  pola[i][j + 1].mina == false)odkryj_pole(i, j + 1, liczbaPol, pola, liczbaZakrytych);

		if (i < (liczbaPol - 1) && j < (liczbaPol - 1)&&pola[i + 1][j+1].mina == false)odkryj_pole(i + 1, j+1, liczbaPol, pola, liczbaZakrytych);

		if (i < (liczbaPol - 1) && j > 0 && pola[i + 1][j - 1].mina == false)odkryj_pole(i + 1, j - 1, liczbaPol, pola, liczbaZakrytych);
		if (i > 0 && j < (liczbaPol - 1)  && pola[i - 1][j + 1].mina == false)odkryj_pole(i - 1, j + 1, liczbaPol, pola, liczbaZakrytych);
	}
	
}