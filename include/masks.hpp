#ifndef CHIP8_MASKS_HPP
#define CHIP8_MASKS_HPP

#include <cstdint>

// Opcode fields extraction masks
constexpr uint16_t MASK_OPC_VX      { 0x0F00u };
constexpr uint16_t MASK_OPC_VY      { 0x00F0u };
constexpr uint16_t MASK_OPC_ADDR    { 0x0FFFu };
constexpr uint16_t MASK_OPC_BYTE    { 0x00FFu };
constexpr uint16_t MASK_OPC_NIBBLE  { 0x000Fu };

// 8-bits registers masks
constexpr uint8_t MASK_LSB          { 0x01u };
constexpr uint8_t MASK_MSB          { 0x80u };
constexpr uint8_t MASK_LOWER_8BITS  { 0xFFu };

#endif