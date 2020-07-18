using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Gamemaster.Models.View
{
    public class SceneView
    {
        public Dictionary<string, Unit> Units { get; set; } = new Dictionary<string, Unit>();

        public SceneView(Scene scene)
        {
            foreach (var u in scene.Units)
            {
                Units.Add(u.Key, u.Value);
            }
        }
    }
}
