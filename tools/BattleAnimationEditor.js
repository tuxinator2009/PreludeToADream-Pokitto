const MAX_FRAMES = 51;

const EFFECT_NONE = 0;
const EFFECT_SPRITE = 1;
const EFFECT_PLAY_SFX = 2;
const EFFECT_SHAKE_SCREEN = 3;
const EFFECT_FLASH_UI = 4;
const EFFECT_FLASH_ENEMY = 5;
const EFFECT_FLASH_PLAYER = 6;

const SPRITE_FLIPPED = 1;
const SPRITE_MIRRORED = 2;

let filePath = window.location.search.substr(1);

class AnimationFrame
{
  constructor()
  {
    this.effect = [0, 0, 0, 0, 0, 0, 0, 0];
    this.value1 = [0, 0, 0, 0, 0, 0, 0, 0];
    this.value2 = [0, 0, 0, 0, 0, 0, 0, 0];
    this.value3 = [0, 0, 0, 0, 0, 0, 0, 0];
    this.value4 = [0, 0, 0, 0, 0, 0, 0, 0];
  }
}

let animation = document.getElementById("animation");
var background = new Image();
var enemies = new Image();
var player = new Image();
var backdrop = new Image();
var animations = new Image();
var frameNumber = new Image();
var spriteIDs = [0, 0, 0, 0, 0, 0, 0, 0];
var frames = [];
var currentSprite = -1;
var spriteOffsetX = 0;
var spriteOffsetY = 0;
var currentFrame = 0;

background.src = "battle.png";
enemies.src = "../resources/battlers/battlers.png";
player.src = "../resources/sprites/player.png";
backdrop.src = "../resources/backdrops/field.png";
animations.src = "../resources/animations/animations.png";
frameNumber.src = "frameNumber.png";

function setup()
{
  for (var i = 0; i < MAX_FRAMES; ++i)
    frames[i] = new AnimationFrame();
  load();
  animation.addEventListener("mousedown", animationMouseDown, false);
  animation.addEventListener("mouseup", animationMouseUp, false);
  animation.addEventListener("mousemove", animationMouseMove, false);
  backdrop.addEventListener("load", render, false);
}

function load()
{
  var fileData = window.readFile(filePath);
  var index = 0;
  if (fileData.length != 2048)
    return;
  for (var i = 0; i < 8; ++i)
    setSprite(i, fileData[index++]);
  for (var i = 0; i < MAX_FRAMES; ++i)
  {
    for (var j = 0; j < 8; ++j)
      frames[i].effect[j] = fileData[index++];
    for (var j = 0; j < 8; ++j)
      frames[i].value1[j] = fileData[index++];
    for (var j = 0; j < 8; ++j)
      frames[i].value2[j] = fileData[index++];
    for (var j = 0; j < 8; ++j)
      frames[i].value3[j] = fileData[index++];
    for (var j = 0; j < 8; ++j)
      frames[i].value4[j] = fileData[index++];
  }
  document.getElementById("frame").value = 1;
  setFrame(1);
  render();
}

function reloadImages()
{
  document.reload();
}

function save()
{
  var fileData = new Uint8Array(2048);
  var index = 0;
  for (var i = 0; i < 8; ++i)
    fileData[index++] = spriteIDs[i];
  for (var i = 0; i < MAX_FRAMES; ++i)
  {
    for (var j = 0; j < 8; ++j)
      fileData[index++] = frames[i].effect[j];
    for (var j = 0; j < 8; ++j)
      fileData[index++] = frames[i].value1[j];
    for (var j = 0; j < 8; ++j)
      fileData[index++] = frames[i].value2[j];
    for (var j = 0; j < 8; ++j)
      fileData[index++] = frames[i].value3[j];
    for (var j = 0; j < 8; ++j)
      fileData[index++] = frames[i].value4[j];
  }
  window.saveFile(filePath, fileData);
}

function setSprite(sprite, value)
{
  var xOffset = (value % 16) * 64;
  var yOffset = (value >> 4) * 64;
  spriteIDs[sprite] = value;
  document.getElementById("sprite" + (sprite + 1) + "_value").value = value;
  document.getElementById("sprite" + (sprite + 1)).style.backgroundPosition = -xOffset + "px " + -yOffset + "px";
}

function changeBackdrop(value)
{
  backdrop.src = "../resources/backdrops/" + value;
}

function setFrame(frame)
{
  currentFrame = frame - 1;
  for (var i = 0; i < 8; ++i)
  {
    document.getElementById("effect" + (i + 1)).value = frames[currentFrame].effect[i];
    setEffect(i, frames[currentFrame].effect[i]);
    document.getElementById("effect" + (i + 1) + "_value1").value = frames[currentFrame].value1[i];
    document.getElementById("effect" + (i + 1) + "_value2").value = frames[currentFrame].value2[i];
    document.getElementById("effect" + (i + 1) + "_value3").value = frames[currentFrame].value3[i];
    document.getElementById("effect" + (i + 1) + "_value4").value = frames[currentFrame].value4[i];
  }
}

function setEffect(effect, value)
{
  var value1 = document.getElementById("effect" + (effect + 1) + "_value1");
  var value2 = document.getElementById("effect" + (effect + 1) + "_value2");
  var value3 = document.getElementById("effect" + (effect + 1) + "_value3");
  var value4 = document.getElementById("effect" + (effect + 1) + "_value4");
  frames[currentFrame].effect[effect] = value;
  if (value == EFFECT_NONE)
  {
    value1.disabled = true;
    value2.disabled = true;
    value3.disabled = true;
    value4.disabled = true;
  }
  else if (value == EFFECT_SPRITE)
  {
    value1.disabled = false;
    value2.disabled = false;
    value3.disabled = false;
    value4.disabled = false;
  }
  else if (value == EFFECT_PLAY_SFX)
  {
    value1.disabled = false;
    value2.disabled = true;
    value3.disabled = true;
    value4.disabled = true;
  }
  else if (value == EFFECT_SHAKE_SCREEN)
  {
    value1.disabled = false;
    value2.disabled = false;
    value3.disabled = true;
    value4.disabled = true;
  }
  else if (value == EFFECT_FLASH_UI)
  {
    value1.disabled = false;
    value2.disabled = false;
    value3.disabled = true;
    value4.disabled = true;
  }
  else if (value == EFFECT_FLASH_ENEMY)
  {
    value1.disabled = false;
    value2.disabled = false;
    value3.disabled = false;
    value4.disabled = false;
  }
  else if (value == EFFECT_FLASH_PLAYER)
  {
    value1.disabled = false;
    value2.disabled = false;
    value3.disabled = false;
    value4.disabled = false;
  }
}

function setValue(effect, valueID, value)
{
  if (valueID == 1)
    frames[currentFrame].value1[effect] = value;
  else if (valueID == 2)
    frames[currentFrame].value2[effect] = value;
  else if (valueID == 3)
    frames[currentFrame].value3[effect] = value;
  else if (valueID == 4)
    frames[currentFrame].value4[effect] = value;
}

function animationMouseDown(e)
{
  var rect = animation.getBoundingClientRect();
  var x = ~~((e.x - rect.x) / 2);
  var y = ~~((e.y - rect.y) / 2);
  for (var i = 0; i < 8; ++i)
  {
    var spriteX = frames[currentFrame].value3[i];
    var spriteY = frames[currentFrame].value4[i];
    if (frames[currentFrame].effect[i] == EFFECT_SPRITE && x >= spriteX && x < spriteX + 32 && y >= spriteY && y < spriteY + 32)
    {
      currentSprite = i;
      spriteOffsetX = spriteX - x;
      spriteOffsetY = spriteY - y;
    }
  }
}

function animationMouseUp(e)
{
  currentSprite = -1;
}

function animationMouseMove(e)
{
  var rect = animation.getBoundingClientRect();
  var x = ~~((e.x - rect.x) / 2);
  var y = ~~((e.y - rect.y) / 2);
  if (currentSprite != -1)
  {
    frames[currentFrame].value3[currentSprite] = x + spriteOffsetX;
    frames[currentFrame].value4[currentSprite] = y + spriteOffsetY;
    if (frames[currentFrame].value3[currentSprite] > 255)
      frames[currentFrame].value3[currentSprite] = 255;
    if (frames[currentFrame].value4[currentSprite] > 255)
      frames[currentFrame].value4[currentSprite] = 255;
    document.getElementById("effect" + (currentSprite + 1) + "_value3").value = frames[currentFrame].value3[currentSprite];
    document.getElementById("effect" + (currentSprite + 1) + "_value4").value = frames[currentFrame].value4[currentSprite];
  }
  render();
}

function isNullFrame()
{
  if (currentFrame >= 85)
    return true;
  for (var i = 0; i < 8; ++i)
  {
    if (frames[currentFrame].effect[i] != EFFECT_NONE)
      return false;
  }
  return true;
}

function playAnimation()
{
  currentFrame = 0;
  play();
}

function play()
{
  if (isNullFrame())
  {
    setFrame(document.getElementById("frame").value);
    render();
    return;
  }
  render(false);
  ++currentFrame;
  setTimeout(play, 100);
}

function render(showFrames=true)
{
  var ctx = animation.getContext("2d");
  var srcX, srcY, srcW, srcH;
  var dstX, dstY, dstW, dstH;
  var monsterID = document.getElementById("monster").value;
  ctx.imageSmoothingEnabled = false;
  ctx.lineWidth = 1;
  ctx.fillStyle = "#404040";
  ctx.fillRect(0, 0, 568, 480);
  ctx.drawImage(background, 64, 64, 440, 352);
  ctx.fillStyle = "#000000";
  ctx.fillRect(63, 63, 442, 1);
  ctx.fillRect(63, 416, 442, 1);
  ctx.fillRect(63, 63, 1, 354);
  ctx.fillRect(503, 63, 1, 354);
  ctx.drawImage(backdrop, 284, 160, 192, 128);
  srcX = (monsterID % 16) * 48;
  srcY = (monsterID >> 4) * 48;
  srcW = 48;
  srcH = 48;
  dstX = 332;
  dstY = 168;
  dstW = 96;
  dstH = 96;
  ctx.drawImage(enemies, srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH);
  srcX = 16;
  srcY = 0;
  srcW = 16;
  srcH = 16;
  dstX = 364;
  dstY = 254;
  dstW = 32;
  dstH = 32;
  ctx.drawImage(player, srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH);
  ctx.strokeStyle = "#0000FF";
  if (currentFrame > 0)
    drawSprites(ctx, frames[currentFrame - 1], showFrames, 7);
  ctx.strokeStyle = "#FF0000";
  drawSprites(ctx, frames[currentFrame], showFrames, 0);
}

function drawSprites(ctx, frame, showFrames, numberOffset)
{
  for (var i = 0; i < 8; ++i)
  {
    if (frame.effect[i] == EFFECT_SPRITE)
    {
      if (showFrames)
      {
        srcX = i * 5;
        srcY = numberOffset;
        srcW = 5;
        srcH = 7;
        dstX = frame.value3[i] * 2 - 2;
        dstY = frame.value4[i] * 2 - 7;
        dstW = 5;
        dstH = 7;
        ctx.strokeRect(frame.value3[i] * 2 - 1, frame.value4[i] * 2 - 1, 66, 66);
        ctx.drawImage(frameNumber, srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH);
        if (numberOffset == 7)
          ctx.globalAlpha = 0.4;
      }
      if (showFrames || numberOffset == 0)
      {
        srcX = (spriteIDs[frame.value1[i]] % 16) * 32;
        srcY = (spriteIDs[frame.value1[i]] >> 4) * 32;
        srcW = 32;
        srcH = 32;
        dstX = frame.value3[i] * 2;
        dstY = frame.value4[i] * 2;
        dstW = 64;
        dstH = 64;
        ctx.save();
        if (frame.value2[i] == 1) //FLIPPED
        {
          ctx.translate(0, dstH);
          ctx.scale(1, -1);
          dstY *= -1;
        }
        else if (frame.value2[i] == 2) //MIRRORED
        {
          ctx.translate(dstW, 0);
          ctx.scale(-1, 1);
          dstX *= -1;
        }
        else if (frame.value2[i] == 3) //FLIPPED+MIRRORED
        {
          ctx.translate(dstW, dstH);
          ctx.scale(-1, -1);
          dstX *= -1;
          dstY *= -1;
        }
        ctx.drawImage(animations, srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH);
        ctx.restore();
      }
      ctx.globalAlpha = 1.0;
    }
  }
}
