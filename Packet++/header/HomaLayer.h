#ifndef PACKETPP_HOMA_LAYER
#define PACKETPP_HOMA_LAYER
 
#include "Layer.h"
 
#define HOMA_DATA_FLAG_MASK        0b1
#define HOMA_ACK_FLAG_MASK         0b10
#define HOMA_NACK_FLAG_MASK        0b100
#define HOMA_GRANT_FLAG_MASK       0b1000
#define HOMA_CHOP_FLAG_MASK        0b10000
#define HOMA_RESEND_FLAG_MASK      0b100000
#define HOMA_BUSY_FLAG_MASK        0b1000000
#define HOMA_BOGUS_FLAG_MASK       0b10000000
 
namespace pcpp
{

#pragma pack(push,1)
    struct homahdr {
        uint8_t flags;
        uint16_t src_context;
        uint16_t dst_context;
        uint16_t msg_len;
        uint8_t pkt_offset;
        uint16_t grant_offset;
        uint8_t grant_prio;
        uint16_t tx_msg_id;
        uint16_t buf_ptr;
        uint8_t buf_size_class;
        uint8_t padding[14];
    };
#pragma pack(pop)
 
 
    class HomaLayer : public Layer
    {
    public:
        HomaLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet);

        HomaLayer(uint8_t flags, uint16_t src_context, uint16_t dst_context, uint16_t msg_len, uint8_t pkt_offset, uint16_t grant_offset, uint8_t grant_prio, uint16_t tx_msg_id, uint16_t buf_ptr, uint8_t buf_size_class);

        ndphdr* getHomaHeader() const { return (homahdr*)m_Data; }

        void parseNextLayer();

        size_t getHeaderLen() const { return sizeof(homahdr); }

        void computeCalculateFields() { return; }

        std::string toString() const;

        OsiModelLayer getOsiModelLayer() const { return OsiModelTransportLayer; }
    };

} // namespace pcpp
 
#endif /* PACKETPP_HOMA_LAYER */