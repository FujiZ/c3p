#include "c3-cs-flow.h"

#include "ns3/log.h"

#include "c3-tag.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("C3CsFlow");

namespace c3p {

NS_OBJECT_ENSURE_REGISTERED (C3CsFlow);

TypeId
C3CsFlow::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::c3p::C3CsFlow")
      .SetParent<C3Flow> ()
      .SetGroupName ("C3p")
      .AddConstructor<C3CsFlow> ()
      .AddAttribute ("WeightMax",
                     "Max weight a flow can get.",
                     DoubleValue (5e-6),
                     MakeDoubleAccessor (&C3CsFlow::m_weightMax),
                     MakeDoubleChecker<double> (0.0))
      .AddAttribute ("WeightMin",
                     "Min weight a flow can get.",
                     DoubleValue (1e-6),
                     MakeDoubleAccessor (&C3CsFlow::m_weightMin),
                     MakeDoubleChecker<double> (0.0))
  ;
  return tid;
}

C3CsFlow::C3CsFlow ()
  : C3Flow (),
    m_weightMax (2.5),
    m_weightMin (0.125)
{
  NS_LOG_FUNCTION (this);
}

C3CsFlow::~C3CsFlow ()
{
  NS_LOG_FUNCTION (this);
}

void
C3CsFlow::Send (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);
  C3Tag c3Tag;
  bool retval = packet->PeekPacketTag (c3Tag);
  NS_ASSERT (retval);
  C3Flow::Send (packet);
}

void
C3CsFlow::UpdateInfo (void)
{
  NS_LOG_FUNCTION (this);
  ///\todo use flowSize instead of remain size
  /// dont use upper/lower bounds
  //m_weight = 1.0 / m_flowSize;
  int32_t remainSize = std::max (m_flowSize - m_sentBytes, m_bufferedBytes);
  ///\todo paper中使用flow size 作为weight，而算法中又是remainsize
  if (remainSize > 0)
    {
      m_weight = std::max (std::min (1.0 / remainSize, m_weightMax), m_weightMin);
    }
  else
    {
      m_weight = 0;
    }
}

double
C3CsFlow::GetWeightMax ()
{
  return m_weightMax;
}

double
C3CsFlow::GetWeightMin ()
{
  return m_weightMin;
}

} //namespace c3p
} //namespace ns3
