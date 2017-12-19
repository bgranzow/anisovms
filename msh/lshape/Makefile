NAME = lshape

all: $(NAME).msh

$(NAME).geo:
	mpicxx -std=c++11 $(NAME).cpp -o $(NAME) -I${GMODEL_DIR}/include -L${GMODEL_DIR}/lib -lgmodel
	./$(NAME)

$(NAME).msh: $(NAME).geo
	gmsh -2 $(NAME).geo -o $(NAME).msh

clean:
	rm -rf $(NAME) $(NAME).geo $(NAME).msh
