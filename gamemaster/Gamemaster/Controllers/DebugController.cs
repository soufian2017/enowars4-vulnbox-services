using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Threading.Tasks;
using Gamemaster.Database;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Gamemaster.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class DebugController : Controller
    {
        private readonly ILogger<DebugController> Logger;
        private readonly IGamemasterDb Db;

        public DebugController(ILogger<DebugController> logger, IGamemasterDb db)
        {
            Logger = logger;
            Db = db;
        }
        [HttpGet]
        public async Task<IActionResult> Test()
        {
            try
            {
                var username = HttpContext.User.Identity.Name;
                var r = new RNGCryptoServiceProvider();
                var r2 = new Random();
                byte[] test = new byte[128];
                byte[] test2 = new byte[128];
                var i = 0;
                foreach (var t in test)
                {
                    test2[i++] =  (byte) (t | (byte)r2.Next());
                }
                r.GetBytes(test);
                var hx = string.Join("", test.Select(c => ((int)c).ToString("X2")));

                return Json(new { hx}) ;
            }
            catch (Exception e)
            {
                Logger.LogError($"{nameof(Test)} failed: {e.Message}");
                return Forbid();
            }
        }
    }
}