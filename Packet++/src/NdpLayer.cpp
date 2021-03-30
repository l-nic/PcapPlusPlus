#include "NdpLayer.h"
#include "AppLayer.h"
#include <string.h>
#include <sstream>

namespace pcpp
{

NdpLayer::NdpLayer(uint8_t flags, uint16_t src_context, uint16_t dst_context,
                     uint16_t msg_len, uint8_t pkt_offset,
                     uint16_t pull_offset, uint16_t tx_msg_id,
                     uint16_t buf_ptr, uint8_t buf_size_class) {
    const size_t header_len = sizeof(ndphdr);
    m_DataLen = header_len;
    m_Data = new uint8_t[header_len];
    memset(m_Data, 0, header_len);
    ndphdr* ndp_hdr = (ndphdr*)m_Data;
    m_Protocol = NDP;
    ndp_hdr->flags = flags;
    ndp_hdr->src_context = htobe16(src_context);
    ndp_hdr->dst_context = htobe16(dst_context);
    ndp_hdr->msg_len = htobe16(msg_len);
    ndp_hdr->pkt_offset = pkt_offset;
    ndp_hdr->pull_offset = htobe16(pull_offset);
    ndp_hdr->tx_msg_id = htobe16(tx_msg_id);
    ndp_hdr->buf_ptr = htobe16(buf_ptr);
    ndp_hdr->buf_size_class = buf_size_class;
}

NdpLayer::NdpLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet): Layer(data, dataLen, prevLayer, packet) {
    m_Protocol = NDP;
    m_DataLen = sizeof(ndphdr) + be16toh(getNdpHeader()->msg_len);
}

void NdpLayer::parseNextLayer() {
    size_t header_len = getHeaderLen();
    if (m_DataLen <= header_len) {
        return;
    }
    uint8_t* payload = m_Data + header_len;
    size_t payload_len = m_DataLen - header_len;
    m_NextLayer = new AppLayer(payload, payload_len, this, m_Packet);
}

std::string NdpLayer::toString() const {
    std::ostringstream output_string;
    std::string flags_str;
    ndphdr* ndp_hdr = getNdpHeader();
    uint8_t ndp_header_flags = ndp_hdr->flags;
    bool is_data = ndp_header_flags & NDP_DATA_FLAG_MASK;
    bool is_ack = ndp_header_flags & NDP_ACK_FLAG_MASK;
    bool is_nack = ndp_header_flags & NDP_NACK_FLAG_MASK;
    bool is_pull = ndp_header_flags & NDP_PULL_FLAG_MASK;
    bool is_chop = ndp_header_flags & NDP_CHOP_FLAG_MASK;
    flags_str += is_data ? "DATA" : "";
    flags_str += is_ack ? " ACK" : "";
    flags_str += is_nack ? " NACK" : "";
    flags_str += is_pull ? " PULL" : "";
    flags_str += is_chop ? " CHOP" : "";
    output_string << "NDP(flags=" << flags_str << ", src_context=" << be16toh(ndp_hdr->src_context)
                  << ", dst_context=" << be16toh(ndp_hdr->dst_context) << ", msg_len="
                  << be16toh(ndp_hdr->msg_len) << ", pkt_offset=" << (int)ndp_hdr->pkt_offset
                  << ", pull_offset=" << be16toh(ndp_hdr->pull_offset) << ", tx_msg_id="
                  << be16toh(ndp_hdr->tx_msg_id) << ", buf_ptr=" << be16toh(ndp_hdr->buf_ptr)
                  << ", buf_size_class=" << (int)ndp_hdr->buf_size_class << ")";
    return output_string.str();
}

}