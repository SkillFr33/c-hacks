#ifndef _INTERFACE_H
#define _INTERFACE_H

/* interface da classe */
struct point {
  // funções-membro:
  
  // funções set
  void (*setX) (struct point* const this, const int _x);
  void (*setY) (struct point* const this, const int _y);
  
  // funções get
  double (*getX) (const struct point* const this);
  double (*getY) (const struct point* const this);
  
  void (*printPoint) (const struct point* const this);

  // membros de dados:
  double x;
  double y;
};

struct _point {
  struct point* (*new) (void);
  void* (*delete) (struct point** object);
};

/* 
  Note que nesse arquivo não são declarados os cabeçalhos das funções-membro da classe,
  isso pois elas não devem ter escopo global. Dessa forma, elas são declaradas como static
  no arquivo de implementação da classe, e o construtor faz com que ponteiros de função da classe
  apontem para essas funções static, sendo assim, possível utiliza-las apenas através de um objeto
*/

extern struct _point Point;

#endif // _INTERFACE_H