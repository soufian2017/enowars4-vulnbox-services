using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Gamemaster.Database;
using Gamemaster.Models.Database;
using System.IO;
using Gamemaster.Models.View;
using Microsoft.EntityFrameworkCore.ChangeTracking;
using System.Diagnostics;
using Microsoft.IO;

namespace Gamemaster.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class AccountController : Controller
    {
        private readonly ILogger<AccountController> Logger;
        private readonly IGamemasterDb Db;
        private static readonly RecyclableMemoryStreamManager StreamManager = new RecyclableMemoryStreamManager();

        public AccountController(ILogger<AccountController> logger, IGamemasterDb db)
        {
            Logger = logger;
            Db = db;
        }

        [HttpPost]
        public async Task<IActionResult> Register([FromForm] string username, [FromForm] string email, [FromForm] string password)
        {
            var foo = new Stopwatch();
            foo.Start();
            var user = await Db.InsertUser(username, email, password);
            foo.Stop();
            var time1 = foo.ElapsedMilliseconds;
            foo.Restart();
            var claims = new List<Claim>
            {
                new Claim(ClaimTypes.Name, username),
                new Claim(ClaimTypes.NameIdentifier, user.Id.ToString())
            };
            var claimsIdentity = new ClaimsIdentity(claims, CookieAuthenticationDefaults.AuthenticationScheme);
            await HttpContext.SignInAsync(new ClaimsPrincipal(claimsIdentity));
            foo.Stop();
            var time2 = foo.ElapsedMilliseconds;
            return Json(new { t1 = time1, t2 = time2 });
            //return new EmptyResult();
        }
        [HttpPost]
        public async Task<IActionResult> Info()
        {
            var currentusername = HttpContext.User.Identity.Name;
            if (currentusername == null)
            {
                return Forbid();
            }
            var currentuser = await Db.GetUserInfo(currentusername);
            if (!(currentuser is User _))
            {
                return StatusCode(StatusCodes.Status500InternalServerError, $"User {currentusername} not found");
            }
            return Json(new UserView (currentuser));
        }
        [HttpPost]
        public async Task<IActionResult> Login([FromForm] string username, [FromForm] string password)
        {
           var dbUser = await Db.AuthenticateUser(username, password);
            if (dbUser is User user)
            {
                var claims = new List<Claim>
                {
                    new Claim(ClaimTypes.Name, username),
                    new Claim(ClaimTypes.NameIdentifier, user.Id.ToString())
                };
                var claimsIdentity = new ClaimsIdentity(claims, CookieAuthenticationDefaults.AuthenticationScheme);
                await HttpContext.SignInAsync( new ClaimsPrincipal(claimsIdentity));
                return new EmptyResult();
            }
            return Forbid();
        }
        [HttpPost]
        public async Task<IActionResult> Logout()
        {
            await HttpContext.SignOutAsync(CookieAuthenticationDefaults.AuthenticationScheme);

            return new EmptyResult();
        }

        [HttpPost]
        public async Task<IActionResult> TestLogin()
        {
            var claims = new List<Claim>
            {
                new Claim(ClaimTypes.Name, "username"),
                new Claim(ClaimTypes.NameIdentifier, "1")
            };

            var claimsIdentity = new ClaimsIdentity(claims, CookieAuthenticationDefaults.AuthenticationScheme);

            var authProperties = new AuthenticationProperties
            {
                //AllowRefresh = <bool>,
                // Refreshing the authentication session should be allowed.

                //ExpiresUtc = DateTimeOffset.UtcNow.AddMinutes(10),
                // The time at which the authentication ticket expires. A 
                // value set here overrides the ExpireTimeSpan option of 
                // CookieAuthenticationOptions set with AddCookie.

                //IsPersistent = true,
                // Whether the authentication session is persisted across 
                // multiple requests. When used with cookies, controls
                // whether the cookie's lifetime is absolute (matching the
                // lifetime of the authentication ticket) or session-based.

                //IssuedUtc = <DateTimeOffset>,
                // The time at which the authentication ticket was issued.

                //RedirectUri = <string>
                // The full path or absolute URI to be used as an http 
                // redirect response value.
            };
            await HttpContext.SignInAsync(
                CookieAuthenticationDefaults.AuthenticationScheme,
                new ClaimsPrincipal(claimsIdentity),
                authProperties);

            return new EmptyResult();
        }
        [HttpPost]
        [RequestSizeLimit(100_000)]
        public async Task<IActionResult> AddToken([FromForm] string name, [FromForm] string description, [FromForm] bool isprivate, [FromForm] IFormFile icon)
        {
            try
            {
                var currentusername = HttpContext.User.Identity.Name;
                if (currentusername == null)
                {
                    return Forbid();
                }
                var currentuser = await Db.GetUser(currentusername);
                if (!(currentuser is User _))
                {
                    throw new Exception($"No user called {currentusername} found");
                }
                using var stream = StreamManager.GetStream();
                await icon.CopyToAsync(stream);
                var iconbin = stream.ToArray();
                var token = await Db.AddTokenToUser(currentuser.Id, name, description, isprivate, iconbin);
                if (token == null)
                {
                    throw new Exception($"Tokencreate failed");
                }
                return Json(token.UUID);
            }
            catch (Exception e)
            {
                Logger.LogError($"{nameof(AddToken)} failed: {e.Message}");
                throw e;
            }
        }
    }
}