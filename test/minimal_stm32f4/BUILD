c_binary('minimal_stm32f4',
  environment = 'stm32f4',
  sources = [ 'main.cc' ],
  deps = [
    '//etl/armv7m:exception_table',
    '//etl/stm32f4xx:interrupt_table',
  ],
  local = {
    'cxx_flags': [
      '-ffreestanding',
    ],
    'link_flags': [
      '-T' + PKG + '/stm32f4xx.ld',
      '-nostartfiles',
    ],
  },
)
