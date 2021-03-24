#ifndef PACKETPP_NDP_LAYER
#define PACKETPP_NDP_LAYER
 
#include "Layer.h"
 
#define NDP_DATA_FLAG_MASK        0b1
#define NDP_ACK_FLAG_MASK         0b10
#define NDP_NACK_FLAG_MASK        0b100
#define NDP_PULL_FLAG_MASK        0b1000
#define NDP_CHOP_FLAG_MASK        0b10000
 
namespace pcpp
{

#pragma pack(push,1)
    struct ndphdr {
        uint8_t flags;
        uint16_t src_context;
        uint16_t dst_context;
        uint16_t msg_len;
        uint8_t pkt_offset;
        uint16_t pull_offset;
        uint16_t tx_msg_id;
        uint16_t buf_ptr;
        uint8_t buf_size_class;
        uint8_t padding[15];
    };
#pragma pack(pop)
 
 
    class NdpLayer : public Layer
    {
    public:
        NdpLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet);

        NdpLayer(uint8_t flags, uint16_t src_context, uint16_t dst_context, uint16_t msg_len, uint8_t pkt_offset, uint16_t pull_offset, uint16_t tx_msg_id, uint16_t buf_ptr, uint8_t buf_size_class);

        ndphdr* getNdpHeader() const { return (ndphdr*)m_Data; }

        void parseNextLayer();

        size_t getHeaderLen() const { return sizeof(ndphdr); }

        void computeCalculateFields() { return; }

        std::string toString() const;

        OsiModelLayer getOsiModelLayer() const { return OsiModelTransportLayer; }
    };

} // namespace pcpp
 
#endif /* PACKETPP_NDP_LAYER */