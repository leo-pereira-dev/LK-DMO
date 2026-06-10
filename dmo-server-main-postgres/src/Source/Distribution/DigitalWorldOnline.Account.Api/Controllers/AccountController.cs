using DigitalWorldOnline.Api.Dtos.Converters;
using DigitalWorldOnline.Api.Dtos.In;
using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Admin.Commands;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.Account;
using DigitalWorldOnline.Commons.Extensions;
using MediatR;
using Microsoft.AspNetCore.Mvc;
using System.Net;

namespace DigitalWorldOnline.Api.Controllers
{
    [ApiController]
    [Route("v1/[controller]")]
    public class AccountController : BaseController
    {
        private readonly ISender _sender;
        private readonly Serilog.ILogger _logger;
        private readonly IConfiguration _configuration;
        public AccountController(
            ISender sender,
            Serilog.ILogger logger,
            IConfiguration configuration)
        {
            _sender = sender;
            _logger = logger;
            _configuration = configuration;
        }

        [ProducesResponseType(201)]
        [ProducesResponseType(401)]
        [HttpPost]
        public async Task<IActionResult> Create(CreateAccountIn account)
        {
            if (GetToken() != _configuration["Authentication:TokenKey"])
            {
                _logger.Information($"Authentication failed for account create request.");
                return Unauthorized();
            }

            try
            {
                var command = CreateAccountCommandConverter.Convert(account);

                var validator = new CreateUserAccountCommandValidator();
                var validationResult = validator.Validate(command);

                if (validationResult.IsValid)
                {
                    var result = await _sender.Send(command);

                    if (result == AccountCreateResult.Created)
                    {
                        _logger.Information($"Account created for username {account.Username}.");
                        return Created("", new { Result = HttpStatusCode.Created });
                    }
                    else
                    {
                        _logger.Information($"Unable to create account for username {account.Username}: {result}.");
                        return Problem(detail: result.GetDescription(), statusCode: GetAccountCreateStatusCode(result));
                    }
                }
                else
                {
                    var validationErrors = string.Join(',', validationResult.Errors.Select(x => x.ErrorMessage));

                    _logger.Information($"Invalid data for account create request. {validationErrors}");

                    return Problem(
                        detail: validationErrors,
                        statusCode: GetAccountCreateStatusCode(AccountCreateResult.InvalidData)
                    );
                }
            }
            catch (Exception ex)
            {
                _logger.Error($"Unexpected error on account create request. Ex.: {ex.Message}. Stack: {ex.StackTrace}");
                return Problem(detail: ex.Message, statusCode: StatusCodes.Status500InternalServerError);
            }
        }

        [ProducesResponseType(201)]
        [HttpPost("register")]
        public async Task<IActionResult> Register(CreateAccountIn account)
        {
            try
            {
                var command = new CreateUserAccountCommand(
                    account.Username,
                    account.Email,
                    account.DiscordId,
                    account.Password);

                var validator = new CreateUserAccountCommandValidator();
                var validationResult = validator.Validate(command);

                if (validationResult.IsValid)
                {
                    var result = await _sender.Send(command);

                    if (result == AccountCreateResult.Created)
                    {
                        _logger.Information($"Site account created for username {account.Username}.");
                        return Created("", new { Result = HttpStatusCode.Created });
                    }

                    _logger.Information($"Unable to create site account for username {account.Username}: {result}.");
                    return Problem(detail: result.GetDescription(), statusCode: GetAccountCreateStatusCode(result));
                }

                var validationErrors = string.Join(',', validationResult.Errors.Select(x => x.ErrorMessage));

                _logger.Information($"Invalid data for site account create request. {validationErrors}");

                return Problem(
                    detail: validationErrors,
                    statusCode: GetAccountCreateStatusCode(AccountCreateResult.InvalidData)
                );
            }
            catch (Exception ex)
            {
                _logger.Error($"Unexpected error on site account create request. Ex.: {ex.Message}. Stack: {ex.StackTrace}");
                return Problem(detail: ex.Message, statusCode: StatusCodes.Status500InternalServerError);
            }
        }

        private static int GetAccountCreateStatusCode(AccountCreateResult result)
        {
            return result switch
            {
                AccountCreateResult.InvalidData => StatusCodes.Status400BadRequest,
                AccountCreateResult.EmailInUse or
                AccountCreateResult.UsernameInUse or
                AccountCreateResult.DiscordInUse => StatusCodes.Status409Conflict,
                _ => StatusCodes.Status500InternalServerError
            };
        }

        [ProducesResponseType(200)]
        [ProducesResponseType(401)]
        [HttpPost("login")]
        public async Task<IActionResult> Login(LoginAccountIn account)
        {
            if (string.IsNullOrWhiteSpace(account.Username) || string.IsNullOrWhiteSpace(account.Password))
            {
                return BadRequest(new { Message = "Usuario e senha sao obrigatorios." });
            }

            try
            {
                var accountDto = await _sender.Send(new AccountByUsernameQuery(account.Username));

                if (accountDto == null || accountDto.Password != account.Password.Encrypt())
                {
                    _logger.Information($"Site login failed for username {account.Username}.");
                    return Unauthorized(new { Message = "Usuario ou senha invalidos." });
                }

                if (accountDto.AccountBlock != null)
                {
                    _logger.Information($"Site login blocked for username {account.Username}.");
                    return Unauthorized(new { Message = "Conta bloqueada." });
                }

                _logger.Information($"Site login success for username {account.Username}.");
                return Ok(new
                {
                    accountDto.Id,
                    accountDto.Username,
                    AccessLevel = accountDto.AccessLevel.ToString()
                });
            }
            catch (Exception ex)
            {
                _logger.Error($"Unexpected error on site login request. Ex.: {ex.Message}. Stack: {ex.StackTrace}");
                return Problem(detail: ex.Message, statusCode: StatusCodes.Status500InternalServerError);
            }
        }

    }
}
