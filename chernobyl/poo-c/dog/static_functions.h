#include "dog_interface.h"

/*
  Esse arquivo contém os protótipos dos métodos da classe
*/

static void _set_peso(void* this, double nome);
static void _latir(void* this);
static const char* const _get_nome(void* this);
static double _get_peso(void* this);
static void _set_nome(void* this, string nome);