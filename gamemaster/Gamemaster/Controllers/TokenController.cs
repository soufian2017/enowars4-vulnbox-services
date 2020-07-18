using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Gamemaster.Database;
using Gamemaster.Models.Database;
using Gamemaster.Models.View;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Gamemaster.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class TokenController : Controller
    {
        private readonly ILogger<TokenController> Logger;
        private readonly IGamemasterDb Db;

        public TokenController(ILogger<TokenController> logger, IGamemasterDb db)
        {
            Logger = logger;
            Db = db;
        }
        [HttpPost]
        public async Task<IActionResult> List()
        {
            try
            {
                var username = HttpContext.User.Identity.Name;
                var user = await Db.GetUser(username);
                var tokens = await Db.GetTokens(user.Id);
                return Json(tokens);
            }
            catch (Exception e)
            {
                Logger.LogError($"{nameof(List)} failed: {e.Message}");
                return Forbid();
            }
        }
        [HttpPost]
        public async Task<IActionResult> Info([FromForm] string UUID)
        {
            TokenStrippedView t = await Db.GetTokenByUUID(UUID);
            if (t.UUID == UUID)
                return Json(t);
            else
                return NotFound();
        }
        [HttpGet]
        public async Task<IActionResult> GetIcon([FromQuery]string UUID)
        {
            TokenData d = await Db.GetTokenDataByUUID(UUID);
            MemoryStream stream = new MemoryStream(d.Icon);
            return new FileStreamResult(stream, "image/png");
        }
    }
}