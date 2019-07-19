/**
 * File:    adrian_single_wire.hpp
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */


#ifndef ADRIAN_SINGLE_WIRE_HPP_
#define ADRIAN_SINGLE_WIRE_HPP_


/* ===== Includes ===== */
#include <stdint.h>


namespace adrian
{
    /**
     * Single Wire Interface
     * Implement this interface for each new platform.
     * This interface can then be used for any project
     * which uses the adrian::SingleWire class.
     * 
     * For more information on single wire, see:
     * https://os.mbed.com/users/fomartin/notebook/n64-controller-interface/
     */
    class SingleWire
    {
    public:
        /* ===== Constants ===== */
        static const uint32_t KHz = 1000;
        static const uint32_t MHz = 1000000;

        /* ===== Enums ===== */

        /** The bit order (big endian or little endian) */
        enum BitOrder
        {
            BIT_ORDER_MSB_FIRST = 0,
            BIT_ORDER_LSB_FIRST = 1,
        };

        /* ===== Functions ===== */

        /** Obligatory virtual destructor */
        virtual ~SingleWire() {};

        /** Initialize the interface (platform dependent) */
        virtual void Initialize(const uint32_t data_pin) = 0;

        /** Release the interface (platform dependent) */
        virtual void Release() = 0;

        /** Set the frequency in Hertz */
        virtual void SetFrequency(const uint32_t frequency) = 0;

        /** Set the bit order (see BitOrder) */
        virtual void SetBitOrder(const BitOrder order) = 0;

        /** Perform a single half-duplex transfer */
        virtual void Transfer(
            const uint8_t tx_buf[],
            const uint8_t num_bytes) = 0;
    };

}   // end namespace adrian


#endif  // ADRIAN_SINGLE_WIRE_HPP_
