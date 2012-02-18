typedef struct {
    unsigned int first;
    unsigned int second;
    unsigned int third;
    unsigned int fourth;
} block;

typedef struct {
    unsigned int first;
    unsigned int second;
    unsigned int third;
    unsigned int fourth;
    unsigned int fifth;
    unsigned int sixth;
    unsigned int seventh;
    unsigned int eighth;
} key;

static int numrounds = 32;
static int numkeys = 33;
static unsigned int phi = 0x9e3779b9;

unsigned char sboxes[][16] = {
	{ 3, 8,15, 1,10, 6, 5,11,14,13, 4, 2, 7, 0, 9,12 },
	{15,12, 2, 7, 9, 0, 5,10, 1,11,14, 8, 6,13, 3, 4 },
	{ 8, 6, 7, 9, 3,12,10,15,13, 1,14, 4, 0,11, 5, 2 },
	{ 0,15,11, 8,12, 9, 6, 3,13, 1, 2, 4,10, 7, 5,14 },
	{ 1,15, 8, 3,12, 0,11, 6, 2, 5, 4,10, 9,14, 7,13 },
	{15, 5, 2,11, 4,10, 9,12, 0, 3,14, 8,13, 6, 7, 1 },
	{ 7, 2,12, 5, 8, 4, 6,11,14, 9, 1,15,13, 3,10, 0 },
	{ 1,13,15, 0,14, 8, 2,11, 7, 4,12,10, 9, 3, 5, 6 }};

unsigned char invsboxes[][16] = {
	{13, 3,11, 0,10, 6, 5,12, 1,14, 4, 7,15, 9, 8, 2 },
	{ 5, 8, 2,14,15, 6,12, 3,11, 4, 7, 9, 1,13,10, 0 },
	{12, 9,15, 4,11,14, 1, 2, 0, 3, 6,13, 5, 8,10, 7 },
	{ 0, 9,10, 7,11,14, 6,13, 3, 5,12, 2, 4, 8,15, 1 },
	{ 5, 0, 8, 3,10, 9, 7,14, 2,12,11, 6, 4,15,13, 1 },
	{ 8,15, 2, 9, 4, 1,13,14,11, 6, 5, 3, 7,12,10, 0 },
	{15,10, 1,13, 5, 3, 6, 0, 4, 9,14, 7, 2,12, 8,11 },
	{ 3, 0, 6,13, 9,14,15, 8, 5,12,11, 7,10, 1, 4, 2 }};

static int initPerm[] = {
    0, 32, 64, 96, 1, 33, 65, 97, 2, 34, 66, 98, 3, 35, 67, 99,
    4, 36, 68, 100, 5, 37, 69, 101, 6, 38, 70, 102, 7, 39, 71, 103,
    8, 40, 72, 104, 9, 41, 73, 105, 10, 42, 74, 106, 11, 43, 75, 107,
    12, 44, 76, 108, 13, 45, 77, 109, 14, 46, 78, 110, 15, 47, 79, 111,
    16, 48, 80, 112, 17, 49, 81, 113, 18, 50, 82, 114, 19, 51, 83, 115,
    20, 52, 84, 116, 21, 53, 85, 117, 22, 54, 86, 118, 23, 55, 87, 119,
    24, 56, 88, 120, 25, 57, 89, 121, 26, 58, 90, 122, 27, 59, 91, 123,
    28, 60, 92, 124, 29, 61, 93, 125, 30, 62, 94, 126, 31, 63, 95, 127
};

static int finalPerm[] = {
    0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
    64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124,
    1, 5, 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61,
    65, 69, 73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125,
    2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62,
    66, 70, 74, 78, 82, 86, 90, 94, 98, 102, 106, 110, 114, 118, 122, 126,
    3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63,
    67, 71, 75, 79, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123, 127  
}; 

#define END 0xff
static unsigned char lineartransform[][8] = {
    {16, 52, 56, 70, 83, 94, 105, END},
    {72, 114, 125, END},
    {2, 9, 15, 30, 76, 84, 126, END},
    {36, 90, 103, END},
    {20, 56, 60, 74, 87, 98, 109, END},
    {1, 76, 118, END},
    {2, 6, 13, 19, 34, 80, 88, END},
    {40, 94, 107, END},
    {24, 60, 64, 78, 91, 102, 113, END},
    {5, 80, 122, END},
    {6, 10, 17, 23, 38, 84, 92, END},
    {44, 98, 111, END},
    {28, 64, 68, 82, 95, 106, 117, END},
    {9, 84, 126, END},
    {10, 14, 21, 27, 42, 88, 96, END},
    {48, 102, 115, END},
    {32, 68, 72, 86, 99, 110, 121, END},
    {2, 13, 88, END},
    {14, 18, 25, 31, 46, 92, 100, END},
    {52, 106, 119, END},
    {36, 72, 76, 90, 103, 114, 125, END},
    {6, 17, 92, END},
    {18, 22, 29, 35, 50, 96, 104, END},
    {56, 110, 123, END},
    {1, 40, 76, 80, 94, 107, 118, END},
    {10, 21, 96, END},
    {22, 26, 33, 39, 54, 100, 108, END},
    {60, 114, 127, END},
    {5, 44, 80, 84, 98, 111, 122, END},
    {14, 25, 100, END},
    {26, 30, 37, 43, 58, 104, 112, END},
    {3, 118, END},
    {9, 48, 84, 88, 102, 115, 126, END},
    {18, 29, 104, END},
    {30, 34, 41, 47, 62, 108, 116, END},
    {7, 122, END},
    {2, 13, 52, 88, 92, 106, 119, END},
    {22, 33, 108, END},
    {34, 38, 45, 51, 66, 112, 120, END},
    {11, 126, END},
    {6, 17, 56, 92, 96, 110, 123, END},
    {26, 37, 112, END},
    {38, 42, 49, 55, 70, 116, 124, END},
    {2, 15, 76, END},
    {10, 21, 60, 96, 100, 114, 127, END},
    {30, 41, 116, END},
    {0, 42, 46, 53, 59, 74, 120, END},
    {6, 19, 80, END},
    {3, 14, 25, 100, 104, 118, END},
    {34, 45, 120, END},
    {4, 46, 50, 57, 63, 78, 124, END},
    {10, 23, 84, END},
    {7, 18, 29, 104, 108, 122, END},
    {38, 49, 124, END},
    {0, 8, 50, 54, 61, 67, 82, END},
    {14, 27, 88, END},
    {11, 22, 33, 108, 112, 126, END},
    {0, 42, 53, END},
    {4, 12, 54, 58, 65, 71, 86, END},
    {18, 31, 92, END},
    {2, 15, 26, 37, 76, 112, 116, END},
    {4, 46, 57, END},
    {8, 16, 58, 62, 69, 75, 90, END},
    {22, 35, 96, END},
    {6, 19, 30, 41, 80, 116, 120, END},
    {8, 50, 61, END},
    {12, 20, 62, 66, 73, 79, 94, END},
    {26, 39, 100, END},
    {10, 23, 34, 45, 84, 120, 124, END},
    {12, 54, 65, END},
    {16, 24, 66, 70, 77, 83, 98, END},
    {30, 43, 104, END},
    {0, 14, 27, 38, 49, 88, 124, END},
    {16, 58, 69, END},
    {20, 28, 70, 74, 81, 87, 102, END},
    {34, 47, 108, END},
    {0, 4, 18, 31, 42, 53, 92, END},
    {20, 62, 73, END},
    {24, 32, 74, 78, 85, 91, 106, END},
    {38, 51, 112, END},
    {4, 8, 22, 35, 46, 57, 96, END},
    {24, 66, 77, END},
    {28, 36, 78, 82, 89, 95, 110, END},
    {42, 55, 116, END},
    {8, 12, 26, 39, 50, 61, 100, END},
    {28, 70, 81, END},
    {32, 40, 82, 86, 93, 99, 114, END},
    {46, 59, 120, END},
    {12, 16, 30, 43, 54, 65, 104, END},
    {32, 74, 85, END},
    {36, 90, 103, 118, END},
    {50, 63, 124, END},
    {16, 20, 34, 47, 58, 69, 108, END},
    {36, 78, 89, END},
    {40, 94, 107, 122, END},
    {0, 54, 67, END},
    {20, 24, 38, 51, 62, 73, 112, END},
    {40, 82, 93, END},
    {44, 98, 111, 126, END},
    {4, 58, 71, END},
    {24, 28, 42, 55, 66, 77, 116, END},
    {44, 86, 97, END},
    {2, 48, 102, 115, END},
    {8, 62, 75, END},
    {28, 32, 46, 59, 70, 81, 120, END},
    {48, 90, 101, END},
    {6, 52, 106, 119, END},
    {12, 66, 79, END},
    {32, 36, 50, 63, 74, 85, 124, END},
    {52, 94, 105, END},
    {10, 56, 110, 123, END},
    {16, 70, 83, END},
    {0, 36, 40, 54, 67, 78, 89, END},
    {56, 98, 109, END},
    {14, 60, 114, 127, END},
    {20, 74, 87, END},
    {4, 40, 44, 58, 71, 82, 93, END},
    {60, 102, 113, END},
    {3, 18, 72, 114, 118, 125, END},
    {24, 78, 91, END},
    {8, 44, 48, 62, 75, 86, 97, END},
    {64, 106, 117, END},
    {1, 7, 22, 76, 118, 122, END},
    {28, 82, 95, END},
    {12, 48, 52, 66, 79, 90, 101, END},
    {68, 110, 121, END},
    {5, 11, 26, 80, 122, 126, END},
    {32, 86, 99, END}
};

static unsigned char inverselineartransform[][8] = {
    {53, 55, 72, END},
    {1, 5, 20, 90, END},
    {15, 102, END},
    {3, 31, 90, END},
    {57, 59, 76, END},
    {5, 9, 24, 94, END},
    {19, 106, END},
    {7, 35, 94, END},
    {61, 63, 80, END},
    {9, 13, 28, 98, END},
    {23, 110, END},
    {11, 39, 98, END},
    {65, 67, 84, END},
    {13, 17, 32, 102, END},
    {27, 114, END},
    {1, 3, 15, 20, 43, 102, END},
    {69, 71, 88, END},
    {17, 21, 36, 106, END},
    {1, 31, 118, END},
    {5, 7, 19, 24, 47, 106, END},
    {73, 75, 92, END},
    {21, 25, 40, 110, END},
    {5, 35, 122, END},
    {9, 11, 23, 28, 51, 110, END},
    {77, 79, 96, END},
    {25, 29, 44, 114, END},
    {9, 39, 126, END},
    {13, 15, 27, 32, 55, 114, END},
    {81, 83, 100, END},
    {1, 29, 33, 48, 118, END},
    {2, 13, 43, END},
    {1, 17, 19, 31, 36, 59, 118, END},
    {85, 87, 104, END},
    {5, 33, 37, 52, 122, END},
    {6, 17, 47, END},
    {5, 21, 23, 35, 40, 63, 122, END},
    {89, 91, 108, END},
    {9, 37, 41, 56, 126, END},
    {10, 21, 51, END},
    {9, 25, 27, 39, 44, 67, 126, END},
    {93, 95, 112, END},
    {2, 13, 41, 45, 60, END},
    {14, 25, 55, END},
    {2, 13, 29, 31, 43, 48, 71, END},
    {97, 99, 116, END},
    {6, 17, 45, 49, 64, END},
    {18, 29, 59, END},
    {6, 17, 33, 35, 47, 52, 75, END},
    {101, 103, 120, END},
    {10, 21, 49, 53, 68, END},
    {22, 33, 63, END},
    {10, 21, 37, 39, 51, 56, 79, END},
    {105, 107, 124, END},
    {14, 25, 53, 57, 72, END},
    {26, 37, 67, END},
    {14, 25, 41, 43, 55, 60, 83, END},
    {0, 109, 111, END},
    {18, 29, 57, 61, 76, END},
    {30, 41, 71, END},
    {18, 29, 45, 47, 59, 64, 87, END},
    {4, 113, 115, END},
    {22, 33, 61, 65, 80, END},
    {34, 45, 75, END},
    {22, 33, 49, 51, 63, 68, 91, END},
    {8, 117, 119, END},
    {26, 37, 65, 69, 84, END},
    {38, 49, 79, END},
    {26, 37, 53, 55, 67, 72, 95, END},
    {12, 121, 123, END},
    {30, 41, 69, 73, 88, END},
    {42, 53, 83, END},
    {30, 41, 57, 59, 71, 76, 99, END},
    {16, 125, 127, END},
    {34, 45, 73, 77, 92, END},
    {46, 57, 87, END},
    {34, 45, 61, 63, 75, 80, 103, END},
    {1, 3, 20, END},
    {38, 49, 77, 81, 96, END},
    {50, 61, 91, END},
    {38, 49, 65, 67, 79, 84, 107, END},
    {5, 7, 24, END},
    {42, 53, 81, 85, 100, END},
    {54, 65, 95, END},
    {42, 53, 69, 71, 83, 88, 111, END},
    {9, 11, 28, END},
    {46, 57, 85, 89, 104, END},
    {58, 69, 99, END},
    {46, 57, 73, 75, 87, 92, 115, END},
    {13, 15, 32, END},
    {50, 61, 89, 93, 108, END},
    {62, 73, 103, END},
    {50, 61, 77, 79, 91, 96, 119, END},
    {17, 19, 36, END},
    {54, 65, 93, 97, 112, END},
    {66, 77, 107, END},
    {54, 65, 81, 83, 95, 100, 123, END},
    {21, 23, 40, END},
    {58, 69, 97, 101, 116, END},
    {70, 81, 111, END},
    {58, 69, 85, 87, 99, 104, 127, END},
    {25, 27, 44, END},
    {62, 73, 101, 105, 120, END},
    {74, 85, 115, END},
    {3, 62, 73, 89, 91, 103, 108, END},
    {29, 31, 48, END},
    {66, 77, 105, 109, 124, END},
    {78, 89, 119, END},
    {7, 66, 77, 93, 95, 107, 112, END},
    {33, 35, 52, END},
    {0, 70, 81, 109, 113, END},
    {82, 93, 123, END},
    {11, 70, 81, 97, 99, 111, 116, END},
    {37, 39, 56, END},
    {4, 74, 85, 113, 117, END},
    {86, 97, 127, END},
    {15, 74, 85, 101, 103, 115, 120, END},
    {41, 43, 60, END},
    {8, 78, 89, 117, 121, END},
    {3, 90, END},
    {19, 78, 89, 105, 107, 119, 124, END},
    {45, 47, 64, END},
    {12, 82, 93, 121, 125, END},
    {7, 94, END},
    {0, 23, 82, 93, 109, 111, 123, END},
    {49, 51, 68, END},
    {1, 16, 86, 97, 125, END},
    {11, 98, END},
    {4, 27, 86, 97, 113, 115, 127, END}
};
