NAME = lshape

all: $(NAME).smb

$(NAME).geo:
	mpicxx -std=c++11 $(NAME).cpp -o $(NAME) -I${GMODEL_DIR}/include -L${GMODEL_DIR}/lib -lgmodel
	./$(NAME)

$(NAME).msh: $(NAME).geo
	gmsh -2 $(NAME).geo -o $(NAME).msh	

$(NAME).smb: $(NAME).msh
	from_gmsh $(NAME).dmg $(NAME).msh $(NAME).smb

clean:
	rm -rf $(NAME) $(NAME).geo $(NAME).dmg $(NAME).msh $(NAME)*.smb out*
