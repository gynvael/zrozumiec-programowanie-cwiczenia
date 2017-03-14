# zrozumiec-programowanie-cwiczenia
## Rozdział 5. Typy pseudorzeczywiste [FPU:real-deal]

### Additional info
[Single Precision](https://en.wikipedia.org/wiki/Single-precision_floating-point_format)

### Conversion
1. **137,9922** converted to binary is **10001001,111111110100100001111111110010111001001000111**.
2. Binary value should be shifted the right by **7** digits, what gives **1.0001001111111110100100001111111110010111001001000111 x 2^7**
3. **K** for SinglePrecision is **127**. Exponent is **127** + **7** what gives **134** and in binary is equal to **10000110**
4. Sign bit is **0**
5. Bits in SinglePrecision:
    
    `{1 signBit}{8 exponentBits}{23 fractionBits}`
    
    **0-10000110-00010011111111101001000**
    
    **4309FF48(hex)**

6. Little endian:
    
    **48FF0943(hex)**  
    
7. Then just run script
    
    ```python single-precision-converter.py 48FF0943```
 

## Prerequisites
- Python
