import cobble

class TestCase(cobble.target.c.Library):
  def __init__(self, *args, **kw):
    super(TestCase, self).__init__(*args, **kw)

    self._local_delta += [
      cobble.env.append('deps', [cobble.Ident.parse('//3p/gtest')]),
      cobble.env.override('whole_archive', True),
    ]

    self._using_delta += [
      cobble.env.append('gtest_cases', [self.identifier])
    ]


class TestRunner(cobble.target.c.Program):
  def _derive_local(self, env_up):
    add_deps = cobble.env.make_appending_delta(
      deps = [self.package.resolve('//3p/gtest')],
    )

    return env_up.derive(self._local_delta + add_deps)


package_verbs = {
  'gtest_case': TestCase,
  'gtest_runner': TestRunner,
}
