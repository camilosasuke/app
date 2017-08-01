

CLASS_DECL_AURA void sleep(const duration & duration)
{
   
   ::nanosleep(static_cast<uint32_t>(duration.total_nanoseconds()));

}

