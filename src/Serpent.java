public class Serpent{

    private static final int numRounds = 32;

    //We need the S-boxes

    private static final int[] initPerm = new int[] {
        0, 32, 64, 96, 1, 33, 65, 97, 2, 34, 66, 98, 3, 35, 67, 99,
        4, 36, 68, 100, 5, 37, 69, 101, 6, 38, 70, 102, 7, 39, 71, 103,
        8, 40, 72, 104, 9, 41, 73, 105, 10, 42, 74, 106, 11, 43, 75, 107,
        12, 44, 76, 108, 13, 45, 77, 109, 14, 46, 78, 110, 15, 47, 79, 111,
        16, 48, 80, 112, 17, 49, 81, 113, 18, 50, 82, 114, 19, 51, 83, 115,
        20, 52, 84, 116, 21, 53, 85, 117, 22, 54, 86, 118, 23, 55, 87, 119,
        24, 56, 88, 120, 25, 57, 89, 121, 26, 58, 90, 122, 27, 59, 91, 123,
        28, 60, 92, 124, 29, 61, 93, 125, 30, 62, 94, 126, 31, 63, 95, 127
    };

    private static final int[] finalPerm = new int[] {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
        64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124,
        1, 5, 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61,
        65, 69, 73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125,
        2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62,
        66, 70, 74, 78, 82, 86, 90, 94, 98, 102, 106, 110, 114, 118, 122, 126,
        3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63,
        67, 71, 75, 79, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123, 127  
    }; 

    private static final long[] s0 = { 
        0x3, 0x8, 0x15, 0x1, 0x10, 0x6, 0x5, 0x11, 
        0x14, 0x13, 0x4, 0x2, 0x7, 0x0, 0x9, 0x12
    };

    private static final long[] s1 = { 
        0x15, 0x12, 0x2, 0x7, 0x9, 0x0, 0x5, 0x10,
        0x1, 0x11, 0x14, 0x8, 0x6, 0x13, 0x3, 0x4
    };

    private static final long[] s2 = { 
        0x8, 0x6, 0x7, 0x9, 0x3, 0x12, 0x10, 0x15, 
        0x13, 0x1, 0x14, 0x4, 0x0, 0x11, 0x5, 0x2
    };

    private static final long[] s3 = { 
        0x0, 0x15, 0x11, 0x8, 0x12, 0x9, 0x6, 0x3,
        0x13, 0x1, 0x2, 0x4, 0x10, 0x7, 0x5, 0x14
    };

    private static final long[] s4 = {
        0x1, 0x15, 0x8, 0x3, 0x12, 0x0, 0x11, 0x6, 
        0x2, 0x5, 0x4, 0x10, 0x9, 0x14, 0x7, 0x13
    };

    private static final long[] s5 = { 
        0x15, 0x5, 0x2, 0x11, 0x4, 0x10, 0x9, 0x12, 
        0x0, 0x3, 0x14, 0x8, 0x13, 0x6, 0x7, 0x1
    };

    private static final long[] s6 = { 
        0x7, 0x2, 0x12, 0x5, 0x8, 0x4, 0x6, 0x11, 
        0x14, 0x9, 0x1, 0x15, 0x13, 0x3, 0x10, 0x0
    };

    private static final long[] s7 = { 
        0x1, 0x13, 0x15, 0x0, 0x14, 0x8, 0x2, 0x11, 
        0x7, 0x4, 0x12, 0x10, 0x9, 0x3, 0x5, 0x6
    };

    private static final long[][] sBoxes = { s0, s1, s2, s3, s4, s5, s6, s7}; 

    private static long[] permute(long upper, long lower){
        return null;
    }

    private static long[] invPermute(long upper, long lower){
        return null;
    }

    private static long[][] subKeyGeneration(long keyUpper, long keyLower){
        return null;
    }

    public static long[] encrypt(long textUpper, long textLower, long keyUpper, long keyLower){
        //Generate round subkeys
        long[][] rndSubkeys = subKeyGeneration(keyUpper, keyLower);
        long[] roundInput = permute(textUpper, textLower);

        for(int i = 0; i < numRounds - 1; i++){
            long mixedUpper = rndSubkeys[i][0] ^ roundInput[0];
            long mixedLower = rndSubkeys[i][1] ^ roundInput[1];

        }

        return null;
    }
}
