SVECTOR model_mesh[] = {
        { -64, -64, 64, 0 },
        { 64, -64, 64, 0 },
        { -64, 64, 64, 0 },
        { 64, 64, 64, 0 },
        { -64, 64, -64, 0 },
        { 64, 64, -64, 0 },
        { -64, -64, -64, 0 },
        { 64, -64, -64, 0 },
};

SVECTOR model_normal[] = {
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, -1, 0 },
        { 0, 0, -1, 0 },
        { 0, 0, -1, 0 },
        { 0, 0, -1, 0 },
        { 0, -1, 0, 0 },
        { 0, -1, 0, 0 },
        { 0, -1, 0, 0 },
        { 0, -1, 0, 0 },
        { 1, 0, 0, 0 },
        { 1, 0, 0, 0 },
        { 1, 0, 0, 0 },
        { 1, 0, 0, 0 },
        { -1, 0, 0, 0 },
        { -1, 0, 0, 0 },
        { -1, 0, 0, 0 },
        { -1, 0, 0, 0 },
};

u_char model_UV[][2] = {
        { 0, 64 },
        { 0, 64 },
        { 64, 64 },
        { 64, 0 },
        { 0, 0 },
        { 0, 0 },
        { 64, 64 },
        { 64, 64 },
        { 64, 0 },
        { 0, 0 },
        { 0, 64 },
        { 0, 0 },
        { 64, 64 },
        { 64, 0 },
        { 64, 64 },
        { 0, 0 },
        { 0, 64 },
        { 64, 64 },
        { 64, 0 },
        { 0, 0 },
        { 0, 64 },
        { 0, 64 },
        { 64, 0 },
        { 64, 0 },
};

Index model_index[] = {
        { 0, 17, 0 },
        { 1, 1, 1 },
        { 2, 18, 3 },
        { 1, 1, 1 },
        { 3, 15, 2 },
        { 2, 18, 3 },
        { 2, 2, 4 },
        { 3, 16, 5 },
        { 4, 22, 7 },
        { 3, 16, 5 },
        { 5, 5, 6 },
        { 4, 22, 7 },
        { 4, 4, 8 },
        { 5, 23, 9 },
        { 6, 20, 11 },
        { 5, 23, 9 },
        { 7, 7, 10 },
        { 6, 20, 11 },
        { 6, 6, 12 },
        { 7, 21, 13 },
        { 0, 8, 15 },
        { 7, 21, 13 },
        { 1, 9, 14 },
        { 0, 8, 15 },
        { 1, 14, 16 },
        { 7, 10, 17 },
        { 3, 3, 19 },
        { 7, 10, 17 },
        { 5, 11, 18 },
        { 3, 3, 19 },
        { 6, 12, 20 },
        { 0, 0, 21 },
        { 4, 13, 23 },
        { 0, 0, 21 },
        { 2, 19, 22 },
        { 4, 13, 23 },
};


